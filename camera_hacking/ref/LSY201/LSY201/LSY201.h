// LSY201.h : main header file for the LSY201 application
//

#if !defined(AFX_LSY201_H__E5769726_C76D_454F_BDF9_FEA116711AEA__INCLUDED_)
#define AFX_LSY201_H__E5769726_C76D_454F_BDF9_FEA116711AEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CLSY201App:
// See LSY201.cpp for the implementation of this class
//

class CLSY201App : public CWinApp
{
public:
	CLSY201App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSY201App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLSY201App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSY201_H__E5769726_C76D_454F_BDF9_FEA116711AEA__INCLUDED_)
