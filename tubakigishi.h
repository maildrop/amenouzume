#pragma once
#if !defined( TUBAKIGISHI_H_HEADER_GUARD )
#define TUBAKIGISHI_H_HEADER_GUARD 1

#if !defined( APP_MAIN_VERSION )
#define APP_MAIN_VERSION (20181012u)
#endif /* !defined( APP_MAIN_VERSION ) */

enum{
  APP_FEATURE_SUCCESS = 0,
  APP_FEATURE_NOT_AVAILABLE = 1,
  APP_FEATURE_NULL_POINTER = 2,
  APP_FEATURE_INVAILD_FUNC = 3,
  APP_FEATURE_INVAILD_PARAM_SIZE = 4,
};

enum{
  APP_FEATURE_DEBUG1,
  APP_FEATURE_ALLOCATOR1,
  APP_FEATURE_LAST
};

enum{
  APP_ACTIVATE_ACTIVATE = 0,
  APP_ACTIVATE_DISACTIVATE = 1,
};

enum LOG_LEVEL{
  LOG_FINEST,
  LOG_FINER,
  LOG_FINE,
  LOG_CONFIG,
  LOG_INFO,
  LOG_WARNING,
  LOG_SEVERE,
};

struct QueryModuleInformation{
  wchar_t display_name[64];
};

struct AppDebug1{
  void (*log)(int log_level , const wchar_t* msg,
              const HANDLE module_handle, const wchar_t* src_name , size_t line );
};

#if !defined( TUBAKIGISHI_HOST )
/* TUBAKIGISHI のコンビニエンス関数 */
uint32_t get_app_serial();
int get_app_feature( int func, void* param , size_t param_size );
HANDLE& get_plugin_handle();
#endif /* !defined( TUBAKIGISHI_HOST ) */

#endif /* !defined( TUBAKIGISHI_H_HEADER_GUARD ) */
