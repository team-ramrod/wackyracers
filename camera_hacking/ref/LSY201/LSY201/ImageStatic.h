#if !defined(AFX_IMAGESTATIC_H__95EA3154_2D5D_424E_9071_8C82DA520766__INCLUDED_)
#define AFX_IMAGESTATIC_H__95EA3154_2D5D_424E_9071_8C82DA520766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageStatic window

#include "Dib.h"
#include "Jpeg.h"


class CImageStatic : public CStatic
{
// Construction
public:
	CImageStatic();

// Attributes
public:
	BOOL DrawImage(LPCTSTR lpszPathName);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL LSY201_OpenFile(LPCTSTR lpszPathName);
	CDib * m_pDib;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGESTATIC_H__95EA3154_2D5D_424E_9071_8C82DA520766__INCLUDED_)
