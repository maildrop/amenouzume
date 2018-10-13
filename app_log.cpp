#include "pch.h"

#include "tubakigishi.h"
#include "app_log.h"

HWND& get_application_log_hwnd()
{
  static HWND hWnd = nullptr;
  return hWnd;
}

struct ApplicationLogElement{
  enum LOG_LEVEL level;
  std::wstring msg;
  HANDLE hModule;
  std::wstring src_name;
  size_t line;
};

static const wchar_t* get_log_level_str( int level );
static enum LOG_LEVEL& get_log_level();
static inline void application_log(const ApplicationLogElement& log_element);

static const wchar_t* get_log_level_str( int level )
{
  switch( level ){
  case LOG_FINEST:
    return L"FINEST";
  case LOG_FINER:
    return L"FINER";
  case LOG_FINE:
    return L"FINE";
  case LOG_CONFIG:
    return L"CONFIG";
  case LOG_INFO:
    return L"INFO";
  case LOG_WARNING:
    return L"WARNING";
  case LOG_SEVERE:
    return L"SEVERE";
  default:
    return L"Unknown" ;
  }
}

static enum LOG_LEVEL& get_log_level()
{
  static enum LOG_LEVEL level = LOG_INFO;
  return level;
}

int application_set_log_level( const HANDLE module_handle , enum LOG_LEVEL new_log_level )
{
  if( module_handle == nullptr ){
    get_log_level() = new_log_level;
    return 0;
  }
  return 1;
}

static inline void application_log(const ApplicationLogElement& log_element)
{
  static std::map<HANDLE,std::wstring> handle_map{};
  std::wstring module_name;
  {
    auto ite = handle_map.find( log_element.hModule );
    if(handle_map.end()== ite){
      std::unique_ptr<wchar_t[]> ptr{new wchar_t[MAX_PATH]};
      if( ::GetModuleFileNameW( HMODULE(log_element.hModule) , ptr.get() , MAX_PATH ) ){
        module_name = PathFindFileNameW(ptr.get());
        handle_map.insert( std::make_pair( log_element.hModule , module_name ) );
      }
    }else{
      module_name = std::get<1>(*ite);
    }
  }
  if( get_log_level() <= log_element.level ){
    std::wstringstream out{};
    out << L"[" << get_log_level_str( log_element.level ) << L"] " 
        << log_element.msg 
        << L" (" << log_element.src_name << L":L." << log_element.line
        << L",@" << module_name.c_str() << L")"
        << std::endl;
    ::OutputDebugStringW( out.str().c_str() );
  }
}

void application_log( int log_level , const wchar_t* msg,
                      const HANDLE module_handle, const wchar_t* src_name , size_t line )
{
  assert( msg );
  assert( module_handle );
  if( msg ){
    if( module_handle ){
      std::unique_ptr<ApplicationLogElement> log_element = 
        std::make_unique<ApplicationLogElement>( );
      {
        log_element->level = (enum LOG_LEVEL)log_level;
        log_element->msg = msg;
        log_element->hModule = module_handle;
        log_element->src_name = src_name ? src_name : L"";
        log_element->line = line;
      }
      if( !get_application_log_hwnd() ){
        application_log( *log_element );
      }
    }
  }
  return;
}



