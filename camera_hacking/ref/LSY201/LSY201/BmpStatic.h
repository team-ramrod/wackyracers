#if !defined(AFX_BMPSTATIC_H__F5C0D259_B359_4D6A_B301_3CAA3364C9BF__INCLUDED_)
#define AFX_BMPSTATIC_H__F5C0D259_B359_4D6A_B301_3CAA3364C9BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpStatic window

class CBmpStatic : public CStatic
{
// Construction
public:
	CBmpStatic();

// Attributes
public:
	HBITMAP hbitmap;
	CBitmap hbmp;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBmpStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPSTATIC_H__F5C0D259_B359_4D6A_B301_3CAA3364C9BF__INCLUDED_)
