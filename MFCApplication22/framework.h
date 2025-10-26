#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Èñêëþ÷èòå ðåäêî èñïîëüçóåìûå êîìïîíåíòû èç çàãîëîâêîâ Windows
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // íåêîòîðûå êîíñòðóêòîðû CString áóäóò ÿâíûìè

// îòêëþ÷àåò ôóíêöèþ ñêðûòèÿ íåêîòîðûõ îáùèõ è ÷àñòî ïðîïóñêàåìûõ ïðåäóïðåæäåíèé MFC
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // îñíîâíûå è ñòàíäàðòíûå êîìïîíåíòû MFC
#include <afxext.h>         // Ðàñøèðåíèÿ MFC


#include <afxdisp.h>        // êëàññû àâòîìàòèçàöèè MFC



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // ïîääåðæêà MFC äëÿ òèïîâûõ ýëåìåíòîâ óïðàâëåíèÿ Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // ïîääåðæêà MFC äëÿ òèïîâûõ ýëåìåíòîâ óïðàâëåíèÿ Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // ïîääåðæêà MFC äëÿ ëåíò è ïàíåëåé óïðàâëåíèÿ









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


