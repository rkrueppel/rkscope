/** @file stdafx.h
* This is the include file for standard system include files, or project specific include files
* that are used frequently, but are changed infrequently. Remember to include $ProjectDir
* in AdditionalIncludeDirectories of the compiler. Since source files are in subfolder and must
* have %include "stdafx.h" (for the compiler to work with precompiled headers, the Intellisense
* parser cannot find stdafx.h since it is not in a subfolder. By including $ProjectDir the parser is happy!\n
* Set the default floating point precision to 64 bit (instead of 53bit) by linking to FP10.OBJ
* in the project properties (do it there because it has to be in the linker command line before
* the msvcrt.lib. */

#pragma once

/** @name Define version of various windows components. Change these values to use different versions
* @{ */
#define WINVER		0x0601
#define _WIN32_WINNT	0x0601
#define _WIN32_IE	0x0800
#define _RICHEDIT_VER	0x0400
/** @} */

/** Include not *everything* */
#define WIN32_LEAN_AND_MEAN

/** Make really sure we are using Unicode */
#ifdef UNICODE						
#undef UNICODE
#endif
#define UNICODE

/** Make really sure we are using strict typechecking */
#ifdef STRICT
#undef STRICT
#endif
#define STRICT

/** Why windef.h defines max and min (small letters!!!) as macros, I do not know... But it is bullshit, so disable them */
#define NOMINMAX					

/** Use math constants */
#define _USE_MATH_DEFINES		

/** disable compiler warning with boost */
#define _SCL_SECURE_NO_WARNINGS

/** Use C++ template overloads of CRT functions. Uses strcpy_s instead of strcpy and handles size count automagically.
* Does not work on temporaries */
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

// For memory leak detection
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

/** Avoid macro redefinition in intsafe vs stdint */
#define _INTSAFE_H_INCLUDED_
#include <stdint.h>

/** Also use floats for DDX */
#define _ATL_USE_DDX_FLOAT

// Compensate for defining NOMINMAX above (atlframe.h complains otherwise...)
#include <algorithm>
using std::max;
using std::min;

// Fundamental Windows stuff
#include <Windows.h>
#include <WindowsX.h>
// needed by atlctrlx.h
#include <ShellAPI.h>

// for COM errors etc
#include <comdef.h>

// ATL includes
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlstr.h>
#include <atlimage.h>
#include <atlcoll.h>
#include <WTypes.h>

/** Do not use WTL's CString (defined in atlmisc.h) */
#define _WTL_NO_CSTRING

/** Do not use WTL's CPoint, CSize, and CRect (defined in atlmisc.h) */
#define _WTL_NO_WTYPES

// WTL includes
#include <atlapp.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include <atlsplit.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atlconv.h>

// Aero (Copied from the WTL samples folder)
#include "gui\ThirdParty\Aero.h"

/** The ATL application module */
extern CAppModule _Module;

// WIC
#include <wincodec.h>
#include <wincodecsdk.h>
#pragma comment(lib, "Windowscodecs.lib")

// Some common STL stuff
#include <array>
#include <sstream>
#include <vector>
#include <map>
#include <numeric>
#include <memory>
#include <deque>
#include <functional>
#include <random>
#include <string>
#include <fstream>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <locale>
#include <codecvt>

// Common Boost stuff
#include <boost\signals2.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\format.hpp>

// Multimedia API (mostly for high accuracy multimedia timers
#include <MMSystem.h>
#pragma comment(lib, "Winmm.lib")

// Direct2D
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib" )

// PSAPI (to get process information like memory consumption)
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

// Common Controls
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Some win32 string stuff
#include <strsafe.h>

/** @name Preprocessor tricks to get the SVN version number in version.h as string
/// @{ */
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
/** @} */

// Disable warning C4345 while using std::array default constructor
#pragma warning(disable:4345)

