// BmpStatic.cpp : implementation file
//

#include "stdafx.h"
#include "lsy201.h"
#include "BmpStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpStatic

CBmpStatic::CBmpStatic()
{
	hbitmap = NULL;
}

CBmpStatic::~CBmpStatic()
{
}


BEGIN_MESSAGE_MAP(CBmpStatic, CStatic)
	//{{AFX_MSG_MAP(CBmpStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpStatic message handlers

void CBmpStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (hbitmap == NULL)
	{
		hbitmap=(HBITMAP)::LoadBitmap(::AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDB_LOGO)); 	
		hbmp.Attach(hbitmap);
	}

	//获取图片格式
	BITMAP bm;
	hbmp.GetBitmap(&bm);
	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *poldBitmap=(CBitmap*)dcMem.SelectObject(hbmp);
	
	CRect lRect;
	GetClientRect(&lRect);
	
	//显示位图
	dc.StretchBlt(lRect.left ,lRect.top ,lRect.Width(),lRect.Height(), 
		&dcMem,0 ,0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	dcMem.SelectObject(&poldBitmap);
}
