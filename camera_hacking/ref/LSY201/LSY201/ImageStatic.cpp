// ImageStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ImageStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "Jpeglib.lib")
#pragma comment(lib, "winmm.lib")

/////////////////////////////////////////////////////////////////////////////
// CImageStatic

CImageStatic::CImageStatic()
{
	m_pDib = new CDib;
}

CImageStatic::~CImageStatic()
{
	if (m_pDib)
	{
		delete m_pDib;
		m_pDib = NULL;
	}
}


BEGIN_MESSAGE_MAP(CImageStatic, CStatic)
	//{{AFX_MSG_MAP(CImageStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageStatic message handlers

//用于打开LSY201的临时文件,其实是一个jpg
BOOL CImageStatic::LSY201_OpenFile(LPCTSTR lpszPathName) 
{
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
	_splitpath(lpszPathName, drive, dir, fname, ext);   
	

	if ( !stricmp(ext, ".jpg") ||
		 ! stricmp(ext, ".jpe") ||
		 ! stricmp(ext, ".jpeg") ||
		 ! stricmp(ext, ".tmp")) // JPEG file
	{
		CJpeg jpeg;
		if (! jpeg.Load(lpszPathName))
			return FALSE;

		HDIB hDIB = CopyHandle(jpeg.GetDib()->GetHandle());
		if (hDIB == NULL)
			return FALSE;

		m_pDib->Attach(hDIB);
	
		return TRUE;
	}
	return FALSE;
}


BOOL CImageStatic::DrawImage(LPCTSTR lpszPathName)
{
	BOOL bRet = FALSE;

	bRet = LSY201_OpenFile(lpszPathName);
	Invalidate(TRUE);
	
	return bRet;
}

void CImageStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectClient;
	GetClientRect(&rectClient);
	dc.FillSolidRect(rectClient, GetSysColor(COLOR_BTNFACE));

	if (!m_pDib->IsEmpty())
	{
		int nWidth = DIBWidth(m_pDib->GetHandle());
		int nHeight = DIBHeight(m_pDib->GetHandle());

		//居中显示
		if (nWidth == 320 && nHeight == 240)
		{
			m_pDib->Display(&dc, 160, 120);
		}
		else if (nWidth == 640 && nHeight == 480)
		{
			m_pDib->Display(&dc, 0, 0);
		}
		else
		{
			m_pDib->Display(&dc, 0, 0);
		}
		
	}	
}

