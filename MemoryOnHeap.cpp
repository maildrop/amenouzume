#include "pch.h"

#include <malloc.h> // CRT 
#define TUBAKIGISHI_HOST 1
#include "tubakigishi.h"
#include "app_log.h"

extern "C"{
  __declspec( dllexport ) uint32_t get_app_serial();
  __declspec( dllexport ) int get_app_feature( int func, void* param , size_t param_size );
  //__declspec( dllexport ) void app_log( int log_level , const wchar_t* msg );
  void* alloc_memory_block(size_t size );
  int free_memory_block(void* ptr);
};

struct AppAllocator1{
  void* (*alloc)(size_t);
  int (*free)(void*);
};

uint32_t get_app_serial()
{
  return APP_MAIN_VERSION;
}

int get_app_feature( int func, void* param , size_t param_size )
{
  if( !param ){
    return APP_FEATURE_NULL_POINTER;
  }
  switch( func ){
  case APP_FEATURE_DEBUG1:
    if( sizeof( AppDebug1 ) == param_size ){
      AppDebug1* debug1 = static_cast<AppDebug1*>( param );
      debug1->log = application_log;
      return APP_FEATURE_SUCCESS;
    }
    return APP_FEATURE_INVAILD_PARAM_SIZE;

  case APP_FEATURE_ALLOCATOR1:
    if( sizeof( AppAllocator1 ) == param_size ){
      AppAllocator1 *allocator = static_cast<AppAllocator1*>( param );
      allocator->alloc = alloc_memory_block;
      allocator->free = free_memory_block;
      return APP_FEATURE_SUCCESS;
    }
    return APP_FEATURE_INVAILD_PARAM_SIZE;

  default:
    return APP_FEATURE_INVAILD_FUNC;
  }
}

static HANDLE get_app_heap_handle()
{
  HANDLE handle = reinterpret_cast<HANDLE>(_get_heap_handle());
  assert( handle );
  return handle;
}

void *alloc_memory_block(size_t size)
{
  return HeapAlloc( get_app_heap_handle() , HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY , size );
}

int free_memory_block(void*ptr)
{
  return HeapFree( get_app_heap_handle() , 0 , ptr );
}


static int entryPoint( int argc , char* argv[] )
{
  (void)(argc);
  (void)(argv);

  std::vector< std::wstring > module_list{};
  do{
    constexpr const size_t path_length{MAX_PATH};
    std::unique_ptr<wchar_t[]> module_path(new wchar_t[path_length] );
    if( GetModuleFileNameW( GetModuleHandle( NULL ) , module_path.get() , path_length ) ){
      if( S_OK != PathCchRemoveFileSpec( module_path.get() , path_length ) ){
        break;
      }
      const std::wstring base_path = module_path.get();

      if( S_OK != PathCchAppendEx( module_path.get() , path_length , L"*.dll" , PATHCCH_NONE ) ){
        break;
      }
      log( module_path.get() );
      {
        std::unique_ptr<WIN32_FIND_DATAW> find_data = std::make_unique<WIN32_FIND_DATAW>();
        HANDLE fff = FindFirstFileW( module_path.get() , find_data.get() );
        if( INVALID_HANDLE_VALUE != fff ){
          do{
            std::unique_ptr<wchar_t[]> path{new wchar_t[MAX_PATH]};
            assert( base_path.length() < MAX_PATH );
            if( S_OK != ::StringCchCopyW( path.get(), MAX_PATH , base_path.c_str() ) ){
              continue;
            }
            if( S_OK != PathCchAppendEx( path.get() , MAX_PATH , find_data->cFileName , PATHCCH_NONE ) ){
              continue;
            }
            ;
            module_list.push_back( std::wstring( path.get() ) );
          }while( FindNextFileW( fff , find_data.get() ) );
          FindClose( fff );
        }
      }
    }
  }while( false );
  log( L"MODULE BEGIN" );
  for( auto& module : module_list ){
    (void)(module);
    log( module.c_str() );
  }
  log( L"MODULE  END " );

  std::vector<std::tuple<HANDLE,int(*)(int,HMODULE),std::wstring>> plugins{};

  std::wstring filename(L"a.dll");
  HINSTANCE module_handle = LoadLibraryExW(filename.c_str() , nullptr , LOAD_LIBRARY_SEARCH_APPLICATION_DIR );
  if(! module_handle ){
    std::cout << "module failed" << std::endl;
  }else{
    plugins.emplace_back(module_handle,(int (*)(int,HMODULE))( GetProcAddress( module_handle,"activate")), filename );
  }
  for( auto& invoke : plugins ){
    if( std::get<1>(invoke) ){
      std::get<1>(invoke)(APP_ACTIVATE_ACTIVATE,static_cast<HMODULE>( std::get<0>(invoke) ));
    }
  }
  for( auto& invoke : plugins ){
    if( std::get<0>(invoke) ){
      if( std::get<1>(invoke) ){
        std::get<1>(invoke)(APP_ACTIVATE_DISACTIVATE,static_cast<HMODULE>(std::get<0>(invoke) ));
      }
      if( !FreeLibrary( static_cast<HMODULE>(std::get<0>(invoke) ))){
        std::cout << "die" << std::endl;
      }
    }
  }
  return EXIT_SUCCESS;
}



int main(int argc ,char* argv[])
{
  { // CRT ヒープの設定
    intptr_t const ptr = _get_heap_handle();
    { // Low Fragment Heap の設定 
      if( GetProcessHeap() != reinterpret_cast<HANDLE>(ptr) ){
        ULONG ulEnableLFH = 2;
        if( !HeapSetInformation( reinterpret_cast<HANDLE>(ptr) ,
                                 HeapCompatibilityInformation ,
                                 &ulEnableLFH ,
                                 sizeof( ulEnableLFH ) ) ){
          // LFH 失敗 
        }
      }
    }
    // Heap 破壊を早期に発見するために、例外を飛ばすようにする。
    if(! HeapSetInformation( reinterpret_cast<HANDLE>(ptr) , HeapEnableTerminationOnCorruption , nullptr , 0 ) ){
      std::cerr << "HeapEanbleTerminationOnCorruption fault" << std::endl;
      return 3;
    }
  }

  std::locale::global( std::locale("") );

  int exitstatus;
  do{
    HRESULT hr = CoInitializeEx( nullptr , COINIT_APARTMENTTHREADED  );
    if( S_OK != hr ){
      exitstatus = 3;
    }else{
      struct CoInitRAII{
        ~CoInitRAII(){
          CoUninitialize();
        }
      } coinit;

      {
        INITCOMMONCONTROLSEX init_commonctrls = {
          sizeof( INITCOMMONCONTROLSEX ),
          ICC_WIN95_CLASSES };
        
        if( ! InitCommonControlsEx(&init_commonctrls) ){
          return 3;
        }
      }

      exitstatus = entryPoint( argc, argv );

    }
  }while( false );
  return exitstatus;
}
