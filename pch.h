#pragma once
#if !defined( PCH_H_HEADER_GUARD )

#include <TCHAR.h>
#include <Windows.h>
#include <commctrl.h>
#include <Shlwapi.h>
#include <pathcch.h>
#include <strsafe.h>

#include <iostream>
#include <locale>
#include <sstream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <deque>
#include <map>
#include <thread>
#include <mutex>
#include <utility>
#include <algorithm>

#include <typeinfo>
#include <type_traits>
#include <cassert>

#pragma comment( lib , "Kernel32.lib" )
#pragma comment( lib , "advapi32.lib" )
#pragma comment( lib , "User32.lib" )
#pragma comment( lib , "Gdi32.lib" )
#pragma comment( lib , "Comctl32.lib" )
#pragma comment( lib , "Ole32.lib" )
#pragma comment( lib , "pathcch.lib" )
#pragma comment( lib , "Shlwapi.lib")

#pragma comment( linker , "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0'    \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#endif /* !defined( PCH_H_HEADER_GUARD ) */
