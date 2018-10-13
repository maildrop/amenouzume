#include "pch.h"
#include "tubakigishi.h"

HANDLE& get_plugin_handle()
{
  static HANDLE plugin_handle = nullptr;
  return plugin_handle;
}

uint32_t get_app_serial()
{
  uint32_t (*get_app_serial_ptr)() =
    reinterpret_cast<uint32_t (*)()>(GetProcAddress( GetModuleHandle( NULL ) , "get_app_serial"));
  if( get_app_serial_ptr ){
    return get_app_serial_ptr();
  }
  return 0u;
}

int get_app_feature( int func, void* param , size_t param_size )
{
  static int (*get_app_feature_ptr)( int , void* , size_t ) =
    reinterpret_cast<int(*)(int,void*,size_t)>( GetProcAddress( GetModuleHandle(NULL), "get_app_feature") );
  if( get_app_feature_ptr ){
    return get_app_feature_ptr( func , param , param_size );
  }
  return APP_FEATURE_NOT_AVAILABLE;
}
