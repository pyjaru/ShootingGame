#pragma once

#define OS_ANDROID	0 // for Android 
#define OS_IOS		1 // for iOS
#define OS_WINDOWS	2 // for Windows

//#define _OS_ 2 // 전처리기에서 정의 !!!!
// _MSC_VER

#if (defined(_DEBUG) && defined(_WIN32))

#endif

#if (_MSC_VER >= 1915)
#	define no_init_all deprecated
#endif

#if (_OS_ == OS_WINDOWS)//(defined(WIN32) || defined(_WIN32))


	//#define _CRT_SECURE_NO_WARNINGS  // for using original c lib (ex- fopen(), va_list)
	//#define _USE_MATH_DEFINES        // for using M_PI

	//#define GLEW_STATIC
#include <gl/glew.h>
#include <gl/wglew.h>
//#include <gl/GL.h>
//#include <gl/GLU.h>
#pragma comment(lib, "opengl32")

#elif (_OS_ == OS_ANDROID)
// jni
// path - 권한
#elif (_OS_ == OS_IOS)
// metal
// path - 권한
#endif 