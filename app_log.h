#pragma once
#if !defined( APP_LOG_H_HEADER_GUARD )
#define APP_LOG_H_HEADER_GUARD 1
#if defined( __cplusplus )

extern "C"{
  void application_log( int log_level , const wchar_t* msg,
                        const HANDLE module_handle, const wchar_t* src_name , size_t line );
  int application_set_log_level( const HANDLE module_handle , enum LOG_LEVEL new_log_level );
  HWND& get_application_log_hwnd();
};



#define WIDE_LITERAL_0( exp ) L ## exp
#define WIDE_LITERAL( exp ) WIDE_LITERAL_0( exp )
#define log( msg ) log_invoke( LOG_INFO , msg , WIDE_LITERAL( __FILE__ ) , __LINE__ )

static inline void log_invoke( int log_level , const wchar_t* msg ,const wchar_t *file_name, size_t line );

static inline void log_invoke( int log_level , const wchar_t* msg ,const wchar_t *file_name, size_t line )
{
  application_log( log_level , msg , GetModuleHandle( NULL ) , file_name , line );
}


#endif /* defined( __cplusplus ) */
#endif /* APP_LOG_H_HEADER_GUARD */
