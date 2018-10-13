#include "pch.h"

#include <cstdint>
#include <iostream>

#include "tubakigishi.h"


extern "C"{
  __declspec(dllexport) int activate(int,HMODULE);
  __declspec(dllexport) int query_module_information(struct QueryModuleInformation* module_info );
};
static int plugin_module_activate( int reason ,HMODULE handle);

int activate(int reason,HMODULE handle)
{
  return plugin_module_activate(reason,handle);
}

static int plugin_module_activate( int reason ,HMODULE handle)
{
  if( 20181012u == get_app_serial() ){
    if( handle != get_plugin_handle() ){
      get_plugin_handle() = handle;
    }
    switch( reason ){
    case APP_ACTIVATE_ACTIVATE:
      {
        AppDebug1 debug1 = {0};
        if( APP_FEATURE_SUCCESS == get_app_feature( APP_FEATURE_DEBUG1 , &debug1 , sizeof( AppDebug1 ) ) ){
          debug1.log( LOG_INFO , L"invoke debug hear" , get_plugin_handle() , L"a.cpp" , __LINE__ );
        }
      }
      return APP_FEATURE_SUCCESS;
    case APP_ACTIVATE_DISACTIVATE:
      return APP_FEATURE_SUCCESS;
    default:
      ;
    }
  }
  return APP_FEATURE_NOT_AVAILABLE;
}
