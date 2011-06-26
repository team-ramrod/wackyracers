// LSY201Dlg.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#include <math.h>

#include <Shlwapi.h>

#include "LSY201.h"
#include "LS_Y201.h"
#include "LSY201Dlg.h"

#pragma comment(lib, "Shlwapi.lib")

extern int g_nSerialNum;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLSY201Dlg dialog

CLSY201Dlg::CLSY201Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLSY201Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLSY201Dlg)
	m_strPath = _T("");
	m_nImageSize = -1;
	m_nTransmitByte = -1;
	m_nSavePower = -1;
	m_nSerialNum = 0;
	m_pLSY201 = NULL;
	m_bPortOpen = FALSE;
	m_nWorkMode = MODE_PHOTO_INVALID;
	pProcessThread = NULL;
	bRun = FALSE;
	hSendCmd = NULL;
	hEventShut = NULL;
	hEventPhotoNext = NULL;
	m_bFileOpen = FALSE;
	m_strDebug = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLSY201Dlg::~CLSY201Dlg()
{
	if(m_pLSY201)
	{
		if (m_pLSY201->IsPortOpen())
		{	
			m_pLSY201->ClosePort();
		}
		
		delete m_pLSY201;
		m_pLSY201 = NULL;
	}
}

void CLSY201Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLSY201Dlg)
	DDX_Control(pDX, IDC_LIST_DEBUG, m_ListDebug);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_COMBO_PHOTO_BAUD, m_ComboPhotoBaud);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_COMBO_PORT_NUM, m_ComboPortNum);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_ComboBaud);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_Radio(pDX, IDC_RADIO_IMAGE_SIZE_320X240, m_nImageSize);
	DDX_Radio(pDX, IDC_RADIO_TRANSMIT_ONCE, m_nTransmitByte);
	DDX_Radio(pDX, IDC_RADIO_SAVE_POWER, m_nSavePower);
	DDX_Text(pDX, IDC_EDIT_SERIAL_NUM, m_nSerialNum);
	DDV_MinMaxInt(pDX, m_nSerialNum, 0, 255);
	DDX_Control(pDX, IDC_LOGO, m_Logo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLSY201Dlg, CDialog)
	//{{AFX_MSG_MAP(CLSY201Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SET_ZIP_RATE, OnBtnSetZipRate)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_ONE_PHOTO, OnBtnOnePhoto)
	ON_BN_CLICKED(IDC_BTN_CONTINUES_PHOTO, OnBtnContinuesPhoto)
	ON_BN_CLICKED(IDC_BTN_STOP_PHOTO, OnBtnStopPhoto)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_CLOSE_PORT, OnBtnClosePort)
	ON_BN_CLICKED(IDC_BTN_OPEN_PORT, OnBtnOpenPort)
	ON_BN_CLICKED(IDC_RADIO_IMAGE_SIZE_320X240, OnRadioImageSize320x240)
	ON_BN_CLICKED(IDC_RADIO_IMAGE_SIZE_640X480, OnRadioImageSize640x480)
	ON_BN_CLICKED(IDC_RADIO_TRANSMIT_ONCE, OnRadioTransmitOnce)
	ON_BN_CLICKED(IDC_RADIO_TRANSMIT_512BYTE, OnRadioTransmit512byte)
	ON_BN_CLICKED(IDC_RADIO_SAVE_POWER, OnRadioSavePower)
	ON_BN_CLICKED(IDC_RADIO_EXIT_SAVE_POWER, OnRadioExitSavePower)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD, OnSelchangeComboBaud)
	ON_BN_CLICKED(IDC_BTN_MODIFY_BAUD, OnBtnModifyBaud)
	ON_BN_CLICKED(IDC_BTN_SAVE_PICTURE, OnBtnSavePicture)
	ON_BN_CLICKED(IDC_BTN_CLEAR_DEBUG, OnBtnClearDebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLSY201Dlg message handlers

BOOL CLSY201Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	ASSERT(m_pLSY201 == NULL);
	m_pLSY201 = new CLSY201(GetSafeHwnd());
	ASSERT(m_pLSY201);

	m_pLSY201->m_pDlg = this;

	InitializeCriticalSection(&m_cs);

	InitValue();

	UpdateData(FALSE);


	StartProcessThread();

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CLSY201Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLSY201Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLSY201Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int strHexToInt(char* strSource)
{ 
	 int nTemp=0;
	 CString strTemp;

	 strTemp=strSource;

	 int nLen = strTemp.GetLength();
	 if (nLen > 2)
	 {
		 if(strSource[0] == '0' && strSource[1] == 'X' ||
			strSource[0] == '0' && strSource[1] == 'x')
		 {
			 CString str;
			 str = strTemp.Right(strTemp.GetLength() - 2);
			 strTemp = str;
		 }		 
	 }
	 
	 for(char cc='G',dd='g';   cc<='Z',dd<='z';  cc++,dd++)    //
	 {
	  if(strTemp.Find(cc,0) !=-1  ||  strTemp.Find(dd,0) !=-1)
	  {
	   ::MessageBox(NULL,"Please input the correct hexadecimal string!","input error",MB_ICONEXCLAMATION);
	   return -1;
	  }
	 }

	 

	 nLen = strTemp.GetLength();
	 char szSource[20];
	 memset(szSource, 0, 20);
	 memcpy(szSource, strTemp.GetBuffer(nLen), nLen);

	 for(int i = 0;  i<(int)::strlen(szSource);  i++)
	 {
	  int nDecNum;
	  switch(szSource[i])
	  {
	   case 'a':
	   case 'A': nDecNum = 10; break;
	   case 'b':
	   case 'B': nDecNum = 11; break;
	   case 'c':
	   case 'C':   nDecNum = 12;   break;
	   case 'd':
	   case 'D':   nDecNum = 13;   break;
	   case 'e':
	   case 'E':   nDecNum = 14;   break;
	   case 'f':
	   case 'F':   nDecNum = 15;   break;
	   case '0':
	   case '1':
	   case '2':
	   case '3':
	   case '4':
	   case '5':
	   case '6':
	   case '7':
	   case '8':
	   case '9':    nDecNum = szSource[i] - '0';     break;
	   default:     return 0;   
			}
		 nTemp += nDecNum * (int)pow(16,::strlen(szSource)-i -1);
	 }
	 return nTemp;
}



void CLSY201Dlg::OnBtnSetZipRate() 
{
	// TODO: Add your control notification handler code here

	//
 	UpdateData(TRUE);
 	if (m_pLSY201)
 	{
		CString strSend;
		GetDlgItem(IDC_EDIT_ZIP_RATE)->GetWindowText(strSend);
		int nValue = strHexToInt((LPSTR)(LPCTSTR)strSend);
		if(nValue !=-1)
		 {
			m_pLSY201->AddCmdToQueue(CMD_ZIP_RATE);
			m_pLSY201->SendSetZipRate(nValue);
		 }
 	}
}

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if(uMsg==BFFM_SELCHANGED||uMsg==BFFM_INITIALIZED)
    {
        if(uMsg==BFFM_INITIALIZED)
        {
            ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,
                LPARAM(lpData));
        }
    }
    return 0;
}

void CLSY201Dlg::OnBtnBrowse() 
{
	CString strFolder;

	LPMALLOC lpMalloc;
 
    if (::SHGetMalloc(&lpMalloc) != NOERROR) 
		return;
 
    char szDisplayName[_MAX_PATH];
    char szBuffer[_MAX_PATH];
    BROWSEINFO browseInfo;
    browseInfo.hwndOwner = NULL;
    browseInfo.pidlRoot = NULL; // set root at Desktop
    browseInfo.pszDisplayName = szDisplayName;
    browseInfo.lpszTitle = "Select a folder";
    browseInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
    browseInfo.lpfn = NULL;
    browseInfo.lParam = 0;
 
    LPITEMIDLIST lpItemIDList;
    if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
    {
        // Get the path of the selected folder from the    item ID list.
        if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
        {
            // At this point, szBuffer contains the path the user chose.
            if (szBuffer[0] == '\0') 
				return;
			
            // We have a path in szBuffer! Return it.
            strFolder = szBuffer;

			m_strPath = strFolder;
			UpdateData(FALSE);

			lpMalloc->Free(lpItemIDList);
			lpMalloc->Release();
            return;
        }
        else 
		{
			lpMalloc->Free(lpItemIDList);
			lpMalloc->Release();

			return; // strResult is empty
		}
	}
}


void CLSY201Dlg::PhotoStop()
{
	if (m_pLSY201)
	{
		m_pLSY201->AddCmdToQueue(CMD_STOP_PHOTO);
		m_pLSY201->SendStopPhoto();
	}	
}

void CLSY201Dlg::PhotoOnce()
{

	if (m_strPath.Right(1) != _T("\\"))
	{
		m_strPath += _T("\\");
	}
	
	m_bCreateNewFile = TRUE;
	m_dwWriteLength = 0;
	m_dwTotalLenth = 0;
	
	if (m_pLSY201)
	{
		ReportInfo("PhotoOnce empty order quene");

		m_pLSY201->Reset();
		m_pLSY201->DeleteAllCmd();

		m_pLSY201->m_bRecvFrameHead = FALSE;
		m_pLSY201->m_bFindFrameEnd = FALSE;

		m_pLSY201->m_nTransmitMode = m_nTransmitByte;
		
		
		m_pLSY201->AddCmdToQueue(CMD_PHOTO);

		
		m_nCurrentStep = STEP_START_PHOTO;
		m_pLSY201->SendPhoto();
	}
}

void CLSY201Dlg::OnBtnOnePhoto() 
{

	UpdateData(TRUE);
	
	

	m_bContinueShot = FALSE;

	m_nWorkMode = MODE_PHOTO_ONCE;

	g_nSerialNum = m_nSerialNum;
	m_pLSY201->SetSerialNum();

	PhotoOnce();

	
	UpdateControls();
}

void CLSY201Dlg::OnBtnContinuesPhoto() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	g_nSerialNum = m_nSerialNum;
	m_pLSY201->SetSerialNum();

	m_bContinueShot = TRUE;

	m_nWorkMode = MODE_PHOTO_CONTINUE;
	UpdateControls();

	PhotoNext();
}

void CLSY201Dlg::PhotoNext()
{
	
	if (m_bContinueShot == FALSE)
	{
		return;
	}


	SetEvent(hEventPhotoNext);
}

void CLSY201Dlg::OnBtnStopPhoto() 
{
	// TODO: Add your control notification handler code here

	m_bContinueShot = FALSE;

	if (m_nWorkMode == MODE_PHOTO_CONTINUE)
	{
		ResetEvent(hEventPhotoNext);
	}

	m_nWorkMode = MODE_PHOTO_INVALID;

	
	if (m_bFileClose == FALSE)
	{
		m_bFileOpen = FALSE;
		m_bFileClose = TRUE;
		m_File.Close();
	}
	
	if(PathFileExists(m_strFileName) == TRUE)
	{
		
		DeleteFile(m_strFileName);
	}

	if (m_pLSY201)
	{
		m_pLSY201->Reset();
		m_pLSY201->DeleteAllCmd();
		m_pLSY201->m_bStart = TRUE;
	}
	
	//PhotoStop();

	m_Progress.SetPos(0);

	UpdateControls();
}

void CLSY201Dlg::OnBtnReset() 
{
	// TODO: Add your control notification handler code here
	if (m_pLSY201)
	{
		m_pLSY201->AddCmdToQueue(CMD_RESET);
		m_pLSY201->SendReset();
	}
}

void CLSY201Dlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here

	DeleteCriticalSection(&m_cs);

	OnOK();
}

void CLSY201Dlg::OnBtnClosePort() 
{
	// TODO: Add your control notification handler code here

	m_pLSY201->DeleteAllCmd();

	ASSERT(m_pLSY201 != NULL);
	m_pLSY201->ClosePort();

	CString strInfo;
	strInfo = _T("Close com");       //--------------------------
	ReportInfo(strInfo);

	GetDlgItem(IDC_BTN_OPEN_PORT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_CLOSE_PORT)->ShowWindow(SW_HIDE);
	
	m_bPortOpen = FALSE;
	UpdateControls();
}

void CLSY201Dlg::OnBtnOpenPort() 
{
	// TODO: Add your control notification handler code here
	
	ASSERT(m_pLSY201 != NULL);
	if (m_pLSY201)
	{
		
		int nPort = m_ComboPortNum.GetCurSel()+1;

		
		UINT nBaud = 0;
		CString strBaud;
		m_ComboBaud.GetLBText(m_ComboBaud.GetCurSel(), strBaud);
		nBaud = atoi(strBaud);
		ASSERT(nBaud != 0);

		BOOL bOpen = m_pLSY201->OpenPort(this, nPort, nBaud);
		if (bOpen)
		{
			CString strInfo;
			strInfo.Format("Open COM%d success, Baud rate %d", nPort, nBaud);
			ReportInfo(strInfo);

			GetDlgItem(IDC_BTN_OPEN_PORT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CLOSE_PORT)->ShowWindow(SW_SHOW);

//			if (m_nImageSize == 0)
//			{
//				OnRadioImageSize320x240();
//			}
//			else if (m_nImageSize == 1)
//			{
//				OnRadioImageSize640x480();
//			}
//			else
//			{
//				ASSERT(FALSE);
//			}
//
//			if (m_nSavePower == 0)
//			{
//				OnRadioSavePower();
//			}
//			else if (m_nSavePower == 1)
//			{
//				OnRadioExitSavePower();
//			}
//			else
//			{
//				ASSERT(FALSE);				
//			}
//
//			OnBtnSetZipRate();
		}
		else
		{
			CString strInfo;
			strInfo.Format("Open COM%d failed, Baud Rate%d", nPort, nBaud);
			ReportError(strInfo);
		}

		m_bPortOpen = bOpen;
		UpdateControls();
	}
	

}

void CLSY201Dlg::OnRadioImageSize320x240() 
{
	UpdateData(TRUE);
	if (m_pLSY201)
	{
		m_pLSY201->AddCmdToQueue(CMD_MODIFY_IMAGE_SIZE);
		m_pLSY201->SendImageSize(MODE_IMAGE_SIZE_320X240);
		Sleep(OPERATE_INTERVAL);
		m_pLSY201->AddCmdToQueue(CMD_RESET);
		m_pLSY201->SendReset();
	}
}

void CLSY201Dlg::OnRadioImageSize640x480() 
{

	if (m_pLSY201)
	{
		m_pLSY201->AddCmdToQueue(CMD_MODIFY_IMAGE_SIZE);
		m_pLSY201->SendImageSize(MODE_IMAGE_SIZE_640X480);
		Sleep(OPERATE_INTERVAL);
		m_pLSY201->AddCmdToQueue(CMD_RESET);
		m_pLSY201->SendReset();
	}	
}

void CLSY201Dlg::OnRadioTransmitOnce() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	

	if (m_pLSY201)
	{
		m_pLSY201->m_nTransmitMode = MODE_TRANSFER_ONCE;
	}
}

void CLSY201Dlg::OnRadioTransmit512byte() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	

	if (m_pLSY201)
	{
		m_pLSY201->m_nTransmitMode = MODE_TRANSFER_512BYTE;
	}
	
}

//Ä¬ÈÏ²ÎÊý
void CLSY201Dlg::InitValue()
{
	m_nImageSize = 0;
	m_nTransmitByte = MODE_TRANSFER_ONCE;
	m_nSavePower = 1;

	m_ComboPortNum.SelectString(-1, _T("COM1"));
	m_ComboBaud.SelectString(-1, _T("38400"));

	m_ComboPhotoBaud.SelectString(-1, _T("38400"));

	GetDlgItem(IDC_EDIT_ZIP_RATE)->SetWindowText("36");
//	m_nZipRate = 36;
	m_strPath = _T("d:\\");

	m_nSerialNum = g_nSerialNum;

	UpdateControls();
}


void CLSY201Dlg::ReportError(CString strError)
{

// 	return;
	
	CString strAdd;
	CTime curTime = CTime::GetCurrentTime();
	strAdd.Format("(%02d:%02d:%02d) Error:%s", 
		curTime.GetHour(),
		curTime.GetMinute(),
		curTime.GetSecond(),
		strError);
	m_ListDebug.ResetContent();
	m_ListDebug.AddString(strAdd);
}

void CLSY201Dlg::ReportInfo(CString strInfo)
{
	CString strAdd;
	CTime curTime = CTime::GetCurrentTime();
	strAdd.Format("(%02d:%02d:%02d) %s", 
		curTime.GetHour(),
		curTime.GetMinute(),
		curTime.GetSecond(),
		strInfo);
	m_ListDebug.ResetContent();
	m_ListDebug.AddString(strAdd);
}

void CLSY201Dlg::OnRadioSavePower() 
{
	UpdateData(TRUE);
	if (m_pLSY201)
	{
		m_pLSY201->AddCmdToQueue(CMD_SAVE_POWER);
		m_pLSY201->SendSavePower(MODE_SAVE_POWER);
	}	
}

void CLSY201Dlg::OnRadioExitSavePower() 
{
	UpdateData(TRUE);
	if (m_pLSY201)
	{
		m_pLSY201->AddCmdToQueue(CMD_EXIT_SAVE_POWER);
		m_pLSY201->SendSavePower(MODE_EXIT_SAVE_POWER);
	}	
}

void CLSY201Dlg::OnSelchangeComboBaud() 
{

}

void CLSY201Dlg::UpdateControls()
{
	
	if (m_bPortOpen == FALSE)
	{
		GetDlgItem(IDC_COMBO_BAUD)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PORT_NUM)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_IMAGE_SIZE_320X240)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_IMAGE_SIZE_640X480)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SAVE_POWER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_EXIT_SAVE_POWER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TRANSMIT_ONCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TRANSMIT_512BYTE)->EnableWindow(FALSE);	
		GetDlgItem(IDC_BTN_SET_ZIP_RATE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_BTN_ONE_PHOTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUES_PHOTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STOP_PHOTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE_PICTURE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_MODIFY_BAUD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_ZIP_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SERIAL_NUM)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PHOTO_BAUD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ZIP_RATE)->EnableWindow(FALSE);
		return;
	}

	if (m_bPortOpen && m_nWorkMode != MODE_PHOTO_INVALID)
	{
		GetDlgItem(IDC_COMBO_BAUD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PORT_NUM)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_IMAGE_SIZE_320X240)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_IMAGE_SIZE_640X480)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TRANSMIT_ONCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TRANSMIT_512BYTE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SAVE_POWER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_EXIT_SAVE_POWER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_ZIP_RATE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_SET_ZIP_RATE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_ONE_PHOTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONTINUES_PHOTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STOP_PHOTO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE_PICTURE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_MODIFY_BAUD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_ZIP_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SERIAL_NUM)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PHOTO_BAUD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ZIP_RATE)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_EXIT)->EnableWindow(TRUE);
		return;
	}

	
	if (m_bPortOpen && m_nWorkMode == MODE_PHOTO_INVALID)
	{
		GetDlgItem(IDC_COMBO_BAUD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PORT_NUM)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_IMAGE_SIZE_320X240)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_IMAGE_SIZE_640X480)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SAVE_POWER)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_EXIT_SAVE_POWER)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TRANSMIT_ONCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TRANSMIT_512BYTE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SET_ZIP_RATE)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_BTN_ONE_PHOTO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CONTINUES_PHOTO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP_PHOTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SAVE_PICTURE)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_MODIFY_BAUD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SET_ZIP_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SERIAL_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PHOTO_BAUD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ZIP_RATE)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_EXIT)->EnableWindow(TRUE);
		return;
	}
}



void CLSY201Dlg::ShowJpegFile()
{

	char szFilter[] = "JPG Files (*.jpg)|*.jpg|JPEG Files (*.jpeg)|*.jpeg|GIF Files (*.gif)|*.gif|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE,"jpg|jpeg|gif",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal() == IDOK)
	{
        CString strFilePath = dlg.GetPathName();
		BOOL bDraw = m_Picture.DrawImage(strFilePath);
		
	}
}

LRESULT CLSY201Dlg::OnProcessThread(WPARAM wParam, LPARAM lParam)
{
	if (lParam == THREAD_FORCE_EXIT)
	{
		ReportError("Serial receiving thread kicked out of!");
	}
	else if (lParam == THREAD_NORMAL_EXIT)
	{
		ReportError("Serial receiving thread kicked out of!");
	}
	else
	{
		ReportError("OnProcessThread unknown lParam !");
	}

	return 0;
}


int	CLSY201Dlg::CreateWriteFile()
{

	CTime curTime = CTime::GetCurrentTime();
	m_strFileName.Format("%sjpeg_%04d%02d%02d_%02d%02d%02d.tmp", m_strPath, 
		curTime.GetYear(), 
		curTime.GetMonth(),
		curTime.GetDay(),
		curTime.GetHour(),
		curTime.GetMinute(),
		curTime.GetSecond());

	int nRet = 0;
	ASSERT(m_bFileOpen == FALSE);

	EnterCriticalSection(&m_cs);
	//reset
	m_dwWriteLength = 0;
	m_bWriteFinish = FALSE;
	LeaveCriticalSection(&m_cs);



	BOOL bOpen = FALSE;
	bOpen = m_File.Open(m_strFileName, CFile::modeCreate |  CFile::modeWrite, NULL);
	ASSERT(bOpen);

	if (bOpen == FALSE)
	{
		nRet = 1;
	}

	//new file have created
	if (bOpen == TRUE)
	{
		m_bCreateNewFile = FALSE;
	}

	m_bFileOpen = bOpen;
	m_bFileClose = FALSE;

	return nRet;
}
//write buffer data to file
int	CLSY201Dlg::WriteBufToFile(BYTE* pBuf, UINT nBufLen)
{
	int nRet = 0;

	BOOL bFinish = FALSE;
	EnterCriticalSection(&m_cs);
	if (m_bWriteFinish == FALSE)
	{
		if (m_dwWriteLength == 0)
		{
			if (nBufLen > 2)
			{
				if (pBuf[0] == 0xFF && pBuf[1] == 0xD8)
				{
					ReportInfo("find JPG frame head");
				}
			}
		}

		if (m_bFileClose == FALSE)
		{
			m_File.Write(pBuf, nBufLen);
			m_dwWriteLength += nBufLen;
		}


		if (m_dwWriteLength == m_dwTotalLenth && m_dwTotalLenth != 0)
		{
			if (nBufLen > 2)
			{
				if (pBuf[nBufLen-2] == 0xFF && pBuf[nBufLen-1] == 0xD9)
				{
					ReportInfo("Find JPG frame tail");
				}
			}
			m_pLSY201->m_bWaitLastFrame = TRUE;
			bFinish = TRUE;
		}
	}
	LeaveCriticalSection(&m_cs);



	if (bFinish)
	{
		m_Progress.SetPos(0);

		EndWriteFile();
		DoShowPicture();

		if (m_nWorkMode == MODE_PHOTO_ONCE)
		{
			m_nWorkMode = MODE_PHOTO_INVALID;
			UpdateControls();
		}
	}
	else
	{
		m_Progress.SetPos(m_dwWriteLength);
	}
// 	else
// 	{
// 		IndRevJpegData();
// 	}

	CString strTrace;
	strTrace.Format("CLSY201Dlg::WriteBufToFile %d, Currently has read  length=%d, total = %d\r\n", 
		nBufLen, m_dwWriteLength, m_dwTotalLenth);
	TRACE(strTrace);

	
	
	return nRet;
}

//file has received
int	CLSY201Dlg::EndWriteFile()
{
	int nRet = 0;

	EnterCriticalSection(&m_cs);
	m_bWriteFinish = TRUE;
	LeaveCriticalSection(&m_cs);
	
	//If the file has been created
	ASSERT(m_bFileOpen == TRUE);
	if (m_bFileOpen == TRUE)
	{
		m_bFileOpen = FALSE;
		m_File.Flush();
		m_File.Close();
		m_bFileClose = TRUE;
	}

	ReportInfo("file has been closed!");
	ASSERT(m_dwWriteLength == m_dwTotalLenth);
		
	
	m_nCurrentStep = STEP_STOP_PHOTO;
	return nRet;
}

void CLSY201Dlg::DoSendReadLength()
{
	m_pLSY201->AddCmdToQueue(CMD_READ_LENGTH);
	
	m_nCurrentStep = STEP_READ_LENGTH;
	
	m_pLSY201->SendReadLength();
}

void CLSY201Dlg::DoShowPicture()
{
	m_Progress.SetPos(0);

	
	if(PathFileExists(m_strCurrentShowPic) == TRUE)
	{
		
		if (FALSE == m_bSavePhto)
		{
			if(FALSE == DeleteFile(m_strCurrentShowPic))
			{
				CString strError;
				strError.Format("Delete %s failed!", m_strCurrentShowPic);
				ReportError(strError);
			}
		}
	}
	m_bSavePhto = FALSE;

	
	m_strCurrentShowPic = m_strFileName;

	BOOL bDraw = m_Picture.DrawImage(m_strFileName);
	if (FALSE == bDraw)
	{
		CString strError;
		strError.Format("Display JPG file %s failed!", m_strFileName);
		ReportError(strError);
	}
}



void CLSY201Dlg::SetFileLength(DWORD dwLength)
{
	EnterCriticalSection(&m_cs);
	m_dwTotalLenth = dwLength;
	LeaveCriticalSection(&m_cs);

	m_Progress.SetRange32(0, m_dwTotalLenth);
	CString strMsg;
	strMsg.Format("m_dwFileLength = %d", m_dwTotalLenth);
	ReportInfo(strMsg);
}


void CLSY201Dlg::IndRevJpegData()
{
	SetEvent(hSendCmd);
}


int CLSY201Dlg::StartProcessThread()
{

	int nRet = 0;

	ASSERT(pProcessThread == NULL);
	ASSERT(bRun == FALSE);


	// create or reset event handle
	if (hEventShut == NULL)
	{
		hEventShut = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	else
	{
		ResetEvent(hEventShut);
	}

	if (hEventPhotoNext == NULL)
	{
		hEventPhotoNext = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	else
	{
		ResetEvent(hEventPhotoNext);
	}

	
	if (hSendCmd == NULL)
	{
		hSendCmd = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	else
	{
		ResetEvent(hSendCmd);
	}

	pProcessThread = AfxBeginThread(ProcessBufProc, this, THREAD_PRIORITY_NORMAL);

	if (pProcessThread == NULL)
	{
		nRet = 1;
		goto ERROR_DAQPROC;
	}


	return nRet;

ERROR_DAQPROC:
	return nRet;
}

int CLSY201Dlg::StopProcessThread()
{
	int nRet = 0;
	int nTimes = 0;

	while((pProcessThread != NULL)&&(bRun))
	{
		SetEvent(hEventShut);
		Sleep(20);

		nTimes += 1;
		if (nTimes >= 50)
		{
			DWORD dwExitCode;
			BOOL  bThreadStatus;
            bThreadStatus = GetExitCodeThread(pProcessThread->m_hThread, &dwExitCode);
            if ((dwExitCode== (DWORD)STILL_ACTIVE) && (bThreadStatus))
            {
				TerminateThread(pProcessThread->m_hThread, 100);
//				CloseHandle(pDaqThread->m_hThread);

				::PostMessage(m_hWnd, UM_PROCESS_THREAD, (WPARAM)0, (LPARAM)THREAD_FORCE_EXIT);
			}
			else
			{
				::PostMessage(m_hWnd, UM_PROCESS_THREAD, (WPARAM)0, (LPARAM)THREAD_NORMAL_EXIT);
			}
			
			break;
		}
	}

	pProcessThread = NULL;
	return nRet;
}


UINT CLSY201Dlg::ProcessBufProc(LPVOID lParam)
{
	UINT nRet = 0;
	CLSY201Dlg *pDlg;
	DWORD dwWaitRes;
	BOOL  bExit;

	DWORD dwLeftLen = 0;
	DWORD dwFileLen = 0;
	DWORD dwReadLen = 0;

	pDlg = (CLSY201Dlg *)lParam;

	pDlg->m_pLSY201->m_bWaitLastFrame = FALSE;

	// set the monitored events
	HANDLE hEventArray[] = {pDlg->hEventShut,
							pDlg->hSendCmd,
							pDlg->hEventPhotoNext};

	DWORD  dwEvCount = sizeof(hEventArray)/sizeof(HANDLE);

	pDlg->bRun = TRUE;
	
	TRACE("CLSY201Dlg::ProcessBufProc is started\n");
	
	bExit = FALSE;

	CString strTrace;
	while(!bExit)
	{

		dwWaitRes = WaitForMultipleObjects(dwEvCount,
                                           hEventArray,
                                           FALSE,
                                           INFINITE);

		CString strTrace;
		strTrace.Format("!!!WaitForMultipleObjects = %d\r\n", dwWaitRes);
		TRACE(strTrace);

		switch(dwWaitRes)
		{
		case 0: // end task
			bExit = TRUE;
			nRet = 2;			// manual terminate
			break;

		case 1: // com data ready

			EnterCriticalSection(&pDlg->m_cs);
			dwFileLen = pDlg->m_dwTotalLenth;
			dwReadLen = pDlg->m_dwWriteLength;
			LeaveCriticalSection(&pDlg->m_cs);

			strTrace.Format("Serial port data ready %d, file length=%d\r\n", 
				dwReadLen, dwFileLen);
			TRACE(strTrace);

			if (pDlg->m_nTransmitByte == MODE_TRANSFER_ONCE) 
			{
				pDlg->m_pLSY201->m_dwRead = dwFileLen; 


				pDlg->m_pLSY201->AddCmdToQueue(CMD_READ_BUF_TOTAL);

				pDlg->m_pLSY201->SendReadBuf(0, dwFileLen);


				strTrace.Format("!!!!once SendReadBuf(0, %d);\r\n", dwFileLen);
				TRACE(strTrace);
			}
			else if (pDlg->m_nTransmitByte == MODE_TRANSFER_512BYTE) 
			{
				if (dwReadLen < dwFileLen)
				{
					dwLeftLen = dwFileLen - dwReadLen;
					if (dwLeftLen > BLOCK_SIZE)
					{
						pDlg->m_pLSY201->m_dwRead = BLOCK_SIZE; 
					}
					else
					{
						pDlg->m_pLSY201->m_dwRead = dwLeftLen; 
					}
					
					pDlg->m_pLSY201->m_bRecvFrameHead = FALSE;
					pDlg->m_pLSY201->m_bFindFrameEnd = FALSE;

					pDlg->m_pLSY201->AddCmdToQueue(CMD_READ_BUF_512);

					
					pDlg->m_pLSY201->SendReadBuf(pDlg->m_dwWriteLength, pDlg->m_pLSY201->m_dwRead);
					
					strTrace.Format("!!!!512Byte once SendReadBuf(%d, %d);\r\n", pDlg->m_dwWriteLength,
						pDlg->m_pLSY201->m_dwRead);
					TRACE(strTrace);		
					
					pDlg->ReportInfo(strTrace);
				}
			}
			else
			{
				ASSERT(FALSE);
			}

			ResetEvent(pDlg->hSendCmd);
			break;

		case 2:
			ASSERT(pDlg->m_nWorkMode == MODE_PHOTO_CONTINUE);
			pDlg->PhotoOnce();
			ResetEvent(pDlg->hEventPhotoNext);
			break;

		case WAIT_FAILED:
			nRet = GetLastError();
			bExit = TRUE;
			break;

		default:
			ASSERT(FALSE);
			bExit = TRUE;
			nRet = 4;
			break;
		}
	} // end while


	pDlg->bRun = FALSE;

	TRACE("CLSY201Dlg::ProcessBufProc is ended with code %d\n", nRet);
	AfxEndThread(nRet);

	return nRet;
}

void CLSY201Dlg::OnBtnModifyBaud() 
{
	// TODO: Add your control notification handler code here
	
	
	UINT nBaud = 0;
	CString strBaud;
	m_ComboPhotoBaud.GetLBText(m_ComboPhotoBaud.GetCurSel(), strBaud);
	nBaud = atoi(strBaud);
	ASSERT(nBaud != 0);


	
	if (m_pLSY201)
	{
		m_pLSY201->AddCmdToQueue(CMD_MODIFY_BAUD);
		m_pLSY201->SendBaud(nBaud);
	}

	

	ReportInfo("Note: modify serial rate does not need to return!");
	

	for (int ii = 0; ii < 10*1000; ++ii)
	{
	}

	OnBtnClosePort();
}

void CLSY201Dlg::OnBtnSavePicture() 
{
	// TODO: Add your control notification handler code here

	m_bSavePhto = TRUE;

	if(PathFileExists(m_strCurrentShowPic) == TRUE)
	{
		if (m_bSavePhto)
		{
			if (m_strCurrentShowPic.IsEmpty() == FALSE)
			{
				int nIndex = m_strCurrentShowPic.ReverseFind('.');
				CString strNewFileName = m_strCurrentShowPic.Left(nIndex);
				strNewFileName += _T(".jpg");
				CFile::Rename(m_strCurrentShowPic, strNewFileName);
			}
		}
	}

}





void CLSY201Dlg::OnBtnClearDebug() 
{
	// TODO: Add your control notification handler code here
	m_ListDebug.ResetContent();
}
