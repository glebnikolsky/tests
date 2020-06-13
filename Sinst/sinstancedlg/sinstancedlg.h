#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CSinstancedlgApp : public CWinApp
{
public:
	CSinstancedlgApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


