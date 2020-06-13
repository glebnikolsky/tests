// CheckCombo.h : main header file for the CHECKCOMBO application
//

#if !defined(AFX_CHECKCOMBO_H__E15F1A84_9C1C_11D3_847A_0050DA346D7F__INCLUDED_)
#define AFX_CHECKCOMBO_H__E15F1A84_9C1C_11D3_847A_0050DA346D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCheckComboApp:
// See CheckCombo.cpp for the implementation of this class
//

class CCheckComboApp : public CWinApp
{
public:
	CCheckComboApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckComboApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCheckComboApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKCOMBO_H__E15F1A84_9C1C_11D3_847A_0050DA346D7F__INCLUDED_)
