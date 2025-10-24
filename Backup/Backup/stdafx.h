// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#undef DEBUG_NEW
	#define  DEBUG_NEW new( __FILE__, __LINE__)
#endif


// TODO: reference additional headers your program requires here
