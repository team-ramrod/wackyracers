// LSY201Dlg.h : header file
//

#if !defined(AFX_LSY201DLG_H__26FA5FAF_0445_4FC3_9876_FCF98E6C2957__INCLUDED_)
#define AFX_LSY201DLG_H__26FA5FAF_0445_4FC3_9876_FCF98E6C2957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLSY201Dlg dialog
#include "resource.h"
#include "ImageStatic.h"
#include "BmpStatic.h"


#define OPERATE_INTERVAL		5

#define STEP_START_PHOTO		1
#define STEP_READ_LENGTH		2
#define STEP_READ_BUF			3
#define STEP_STOP_PHOTO			4

class CLSY201;

class CLSY201Dlg : public CDialog
{
// Construction
public:
	CLSY201Dlg(CWnd* pParent = NULL);	// standard constructor
	~CLSY201Dlg();
// Dialog Data
	//{{AFX_DATA(CLSY201Dlg)
	enum { IDD = IDD_LSY201_DIALOG };
	CListBox	m_ListDebug;
	CProgressCtrl	m_Progress;
	CComboBox	m_ComboPhotoBaud;
	CImageStatic m_Picture;
	CComboBox	m_ComboPortNum;
	CComboBox	m_ComboBaud;
	CString		m_strPath;
	int		m_nImageSize;
	int		m_nTransmitByte;
	int		m_nSavePower;
	int		m_nSerialNum;
	CBmpStatic m_Logo;
	CString	m_strDebug;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSY201Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitValue();
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CLSY201Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSetZipRate();
	afx_msg void OnBtnBrowse();
	afx_msg void OnBtnOnePhoto();
	afx_msg void OnBtnContinuesPhoto();
	afx_msg void OnBtnStopPhoto();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnClosePort();
	afx_msg void OnBtnOpenPort();
	afx_msg void OnRadioImageSize320x240();
	afx_msg void OnRadioImageSize640x480();
	afx_msg void OnRadioTransmitOnce();
	afx_msg void OnRadioTransmit512byte();
	afx_msg void OnRadioSavePower();
	afx_msg void OnRadioExitSavePower();
	afx_msg void OnSelchangeComboBaud();
	afx_msg void OnBtnModifyBaud();
	afx_msg void OnBtnSavePicture();
	afx_msg void OnBtnClearDebug();
	//}}AFX_MSG
	afx_msg LRESULT OnProcessThread(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:

	CString     m_strCurrentShowPic;

	BOOL		m_bContinueShot;

	BOOL		m_nWorkMode; 

	BOOL		m_bFileClose;
	BOOL		m_bFileOpen;
	BOOL		m_bPortOpen;
	CLSY201*	m_pLSY201;

	BOOL		m_bSavePhto;
	void	ReportInfo(CString strInfo);
	void	ReportError(CString strError);

	void	PhotoNext();

	void	SetFileLength(DWORD dwLength);


	int		CreateWriteFile();

	int		WriteBufToFile(BYTE* pBuf, UINT nBufLen);

	
	int		EndWriteFile();

	void	DoShowPicture();
	
	void	DoSendReadLength();


	void	IndRevJpegData();

	void	PhotoOnce();

	void    PhotoStop();
private:


	void	UpdateControls();

	CFile	m_File;
	CString m_strFileName; 
	BOOL	m_bCreateNewFile; 
	DWORD	m_dwTotalLenth; 
	DWORD	m_dwWriteLength; 
	BOOL	m_bWriteFinish;

	void	ShowJpegFile();


	int		m_nCurrentStep;

	static	UINT	ProcessBufProc(LPVOID LParam); 
	int				StartProcessThread();
	int 			StopProcessThread();

	HANDLE			hEventPhotoNext;	
	HANDLE          hEventShut;			// close thread handle
	HANDLE			hSendCmd;		// serial data ready handle
	CWinThread		*pProcessThread;	// pointer to thread
	BOOL			bRun;

	CRITICAL_SECTION	m_cs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSY201DLG_H__26FA5FAF_0445_4FC3_9876_FCF98E6C2957__INCLUDED_)
