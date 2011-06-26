// LSY2011.cpp: implementation of the CLSY201 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LSY201.h"
#include "LS_Y201.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern int g_nSerialNum;

//���к��ڵ�2���ֽ�
#define ID_SERIAL_NUM		1

//��λ --- 56 00 26 00
//���� --- 76 00 26 00
BYTE cByteReset[] = 
{
	0x56,
	0x00,
	0x26,
	0x00,
}; 

BYTE cByteRetReset[] = 
{
	0x76,
	0x00,
	0x26,
	0x00,
}; 


//���� --- 56 00 36 01 00
//���� --- 76 00 36 00 00
BYTE cBytePhoto[] = 
{
	0x56,
	0x00,
	0x36,
	0x01,
	0x00,
}; 

BYTE cByteRetPhoto[] = 
{
	0x76,
	0x00,
	0x36,
	0x00,
	0x00,
}; 

//������ --- 56 00 34 01 00 
//����   --- 76 00 34 00 04 xx xx xx xx
BYTE cByteReadLength[] = 
{
	0x56,
	0x00,
	0x34,
	0x01,
	0x00,
}; 

BYTE cByteRetReadLength[] = 
{
	0x76,
	0x00,
	0x34,
	0x00,
	0x04,
	0x00, //���ȸ��ֽ�
	0x00, //
	0x00, //
	0x00, //���ȵ��ֽ�
}; 

//��buf  --- 56 00 32 0C 00 0A 00 00 MM MM 00 00 KK KK XX XX 
//����   --- 76 00 34 00 04 xx xx xx xx
BYTE cByteReadBuf[] = 
{
	0x56,
	0x00,
	0x32,
	0x0C,
	0x00,
	0x0A,

	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
}; 

//ֹͣ���� --- 56 00 36 01 03
//����	   --- 76 00 36 00 00
BYTE cByteStopPhoto[] = 
{
	0x56,
	0x00,
	0x36,
	0x01,
	0x03,
}; 

BYTE cByteRetStopPhoto[] = 
{
	0x76,
	0x00,
	0x36,
	0x00,
	0x00,
}; 

//�ϵ� 
const BYTE cByteRetOnPower[] = 
{
// 0x56,
// 0x43,
// 0x30, 
// 0x37,
// 0x30, 
// 0x33, 
// 0x20, 
// 0x31, 
// 0x2E, 
// 0x30, 
// 0x30, 
// 0x0D, 
// 0x0A, 
// 0x43, 
// 0x74, 
// 0x72, 
// 0x6C, 
// 0x20, 
// 0x69, 
// 0x6E, 
// 0x66, 
// 0x72, 
// 0x20, 
// 0x65, 
// 0x78, 
// 0x69, 
// 0x73, 
// 0x74, 
// 0x0D, 
// 0x0A, 
// 0x55, 
// 0x73, 
// 0x65, 
// 0x72, 
// 0x2D,
// 0x64, 
// 0x65, 
// 0x66, 
// 0x69, 
// 0x6E, 
// 0x65, 
// 0x64, 
// 0x20, 
// 0x73, 
// 0x65, 
// 0x6E, 
// 0x73, 
// 0x6F, 
// 0x72, 
// 0x0D, 
// 0x0A, 
// 0x36, 
// 0x32, 
// 0x35, 
// 0x0D, 
// 0x0A, 
0x49, 
0x6E, 
0x69, 
0x74, 
0x20, 
0x65, 
0x6E, 
0x64, 
0x0D, 
0x0A, 
}; 

const int nSizeRetOnPower = sizeof(cByteRetOnPower) / sizeof(BYTE);


//ͼ���С(320*240) --- 56 00 31 05 04 01 00 19 11
//����			    --- 76 00 31 00 00
BYTE cByteImageSize320x240[] = 
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x04,
	0x01,
	0x00,
	0x19,
	0x11,
}; 

BYTE cByteRetImageSize320x240[] = 
{
	0x76,
	0x00,
	0x31,
	0x00,
	0x00,
}; 


//ͼ���С(640*480) --- 56 00 31 05 04 01 00 19 00
//����			    --- 76 00 31 00 00
//ע��: ��Ҫ��λ
BYTE cByteImageSize640x480[] = 
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x04,
	0x01,
	0x00,
	0x19,
	0x00,
}; 

BYTE cByteRetImageSize640x480[] = 
{
	0x76,
	0x00,
	0x31,
	0x00,
	0x00,
}; 

//ʡ�� --- 56 00 3E 03 00 01 01
//���� --- 76 00 3E 00 00
BYTE cByteSavePower[] = 
{
	0x56,
	0x00,
	0x3E,
	0x03,
	0x00,
	0x01,
	0x01,
}; 

BYTE cByteRetSavePower[] = 
{
	0x76,
	0x00,
	0x3E,
	0x00,
	0x00,
}; 

//�˳�ʡ�� --- 56 00 3E 03 00 01 00
//����	   --- 76 00 3E 00 00
BYTE cByteExitSavePower[] = 
{
	0x56,
	0x00,
	0x3E,
	0x03,
	0x00,
	0x01,
	0x00,
}; 

BYTE cByteRetExitSavePower[] = 
{
	0x76,
	0x00,
	0x3E,
	0x00,
	0x00,
}; 


BYTE cByteBaud9600[] = 
{
	0x56,
	0x00,
	0x24,
	0x03,
	0x01,
	0xAE,
	0xC8,
}; 

BYTE cByteBaud19200[] = 
{
	0x56,
	0x00,
	0x24,
	0x03,
	0x01,
	0x56,
	0xE4,
}; 

BYTE cByteBaud38400[] = 
{
	0x56,
	0x00,
	0x24,
	0x03,
	0x01,
	0x2A,
	0xF2,
}; 

BYTE cByteBaud57600[] = 
{
	0x56,
	0x00,
	0x24,
	0x03,
	0x01,
	0x1C,
	0x4C,
}; 

BYTE cByteBaud115200[] = 
{
	0x56,
	0x00,
	0x24,
	0x03,
	0x01,
	0x0D,
	0xA6,
}; 

BYTE cByteRetBaud[] = 
{
	0x76,
	0x00,
	0x24,
	0x00,
	0x00,
}; 


//ѹ���� --- 56 00 31 05 01 01 12 04 XX
//����   --- 76 00 31 00 00
BYTE cByteZipRate[] = 
{
	0x56,
	0x00,
	0x31,
	0x05,
	0x01,
	0x01,
	0x12,
	0x04,
	0x00,  //�޸�Ϊ��Ӧ��ѹ����, Ĭ��36 
}; 

BYTE cByteRetZipRate[] = 
{
	0x76,
	0x00,
	0x31,
	0x00,
	0x00,
}; 


// 4��������56 00 32 0C 00 0A 00 00 MM MM 00 00 KK KK XX XX
// ���أ�76 00 32 00 00 �����ʱ�䣩   FF D8  ���������������������ʱ�䣩 76 00 32 00 00
// 
// �����ʱ�䣩= XX  XX*0��01����
// MM  MM  MM  MM  ��ʼ��ַ
// KK  KK   KK   KK     ���ݳ���
// 


// ��λ
// 
// 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLSY201::CLSY201(HWND hParent)
{
	m_hParentWnd = hParent;
	pProcessThread = NULL;
	bRun = FALSE;

	hEventShut = NULL;

	m_bPortOpen = FALSE;
	InitializeCriticalSection(&m_csBuf);
	InitializeCriticalSection(&m_csCmdQueue);
}

CLSY201::~CLSY201()
{
	DeleteCriticalSection(&m_csBuf);
	DeleteCriticalSection(&m_csCmdQueue);
}

//���ڴ򿪺͹ر�
BOOL CLSY201::OpenPort(CWnd* pPortOwner, UINT portnr, UINT baud, char parity, UINT databits, UINT stopsbits, DWORD dwCommEvents, UINT nBufferSize)
{
	
	BOOL bOpen = FALSE;
	bOpen = m_SerialPort.InitPort(pPortOwner, portnr, baud, parity, databits, stopsbits, dwCommEvents, nBufferSize);
	
	if (bOpen)
	{
		//���������մ����߳�
		StartProcessThread();
		m_SerialPort.StartMonitoring();
	}

	m_bPortOpen = bOpen;
	
	m_bStart = TRUE;
	return bOpen;
}

void CLSY201::ClosePort()
{
	//�ȹرմ��ڷ����߳�
	m_SerialPort.StopMonitoring();
	StopProcessThread();

	//ɾ������������
	EnterCriticalSection(&m_csBuf);
	m_ArrBuf.RemoveAll();
	LeaveCriticalSection(&m_csBuf);

	m_bPortOpen = FALSE;

	m_bStart = FALSE;
}

BOOL CLSY201::IsPortOpen()
{
	return m_bPortOpen;
}

//���ݷ��͵ķ�װ
void CLSY201::SendReset()
{
	m_SerialPort.WriteToPort(cByteReset, sizeof(cByteReset)/sizeof(BYTE));
}

void CLSY201::SendPhoto()
{
//	AfxMessageBox("������������");
	m_SerialPort.WriteToPort(cBytePhoto, sizeof(cBytePhoto)/sizeof(BYTE));	
}

void CLSY201::SendReadLength()
{
	m_SerialPort.WriteToPort(cByteReadLength, sizeof(cByteReadLength)/sizeof(BYTE));		
}

void CLSY201::SendReadBuf(DWORD dwStartAddr, DWORD dwLength, UINT nInterval/* = 255*/)
{
	//3~0�Ӹ�λ����λ
	BYTE byStartAddr[4];
	BYTE byLength[4];
	BYTE byInterval[2];

	byStartAddr[3] = (BYTE)(dwStartAddr / 0x1000000);
	byStartAddr[2] = (BYTE)((dwStartAddr % 0x1000000) / 0x10000);
	byStartAddr[1] = (BYTE)(((dwStartAddr % 0x1000000) % 0x10000) / 0x100);
	byStartAddr[0] = (BYTE)(((dwStartAddr % 0x1000000) % 0x10000) % 0x100);
	
	byLength[3] = (BYTE)(dwLength / 0x1000000);
	byLength[2] = (BYTE)((dwLength % 0x1000000) / 0x10000);
	byLength[1] = (BYTE)(((dwLength % 0x1000000) % 0x10000) / 0x100);
	byLength[0] = (BYTE)(((dwLength % 0x1000000) % 0x10000) % 0x100);
	
	byInterval[1] = (BYTE)(nInterval / 0x100);
	byInterval[0] = (BYTE)(nInterval % 0x100);

	int nSize = sizeof(cByteReadBuf)/sizeof(BYTE);
	
	//�ٶ��˴�Э�鳤�Ȳ��ᳬ��20
	ASSERT(nSize < 20);
	
	BYTE byReadBuf[20];
	memset(byReadBuf, 0, 20);
	memcpy(byReadBuf, cByteReadBuf, nSize);
/*	
	const BYTE cByteReadBuf[] = 
	{
		0x56,
		0x00,
		0x32,
		0x0C,
		0x00,
		0x0A,
		
		0x00,
		0x00,
		0x00,
		0x00,
		
		0x00,
		0x00,
		0x00,
		0x00,
		
		0x00,
		0x00,
	}; 
*/
	byReadBuf[6] = byStartAddr[3];
	byReadBuf[7] = byStartAddr[2];
	byReadBuf[8] = byStartAddr[1];
	byReadBuf[9] = byStartAddr[0];
	
	byReadBuf[10] = byLength[3];
	byReadBuf[11] = byLength[2];
	byReadBuf[12] = byLength[1];
	byReadBuf[13] = byLength[0];

	byReadBuf[14] = byInterval[1];
	byReadBuf[15] = byInterval[0];
	
	
	m_SerialPort.WriteToPort(byReadBuf, nSize);

}

void CLSY201::SendStopPhoto()
{
//	AfxMessageBox("����ֹͣ��������");
	
	m_SerialPort.WriteToPort(cByteStopPhoto, sizeof(cByteStopPhoto)/sizeof(BYTE));			
}

void CLSY201::SendSetZipRate(int nZipRate)
{
	int nSize = sizeof(cByteZipRate)/sizeof(BYTE);

	//�ٶ��˴�Э�鳤�Ȳ��ᳬ��20
	ASSERT(nSize < 20);

	BYTE byZipRate[20];
	memset(byZipRate, 0, 20);
	memcpy(byZipRate, cByteZipRate, nSize);

	byZipRate[nSize-1] = nZipRate;
	
	
	m_SerialPort.WriteToPort(byZipRate, nSize);
}

void CLSY201::SendImageSize(int nModeImageSize)
{
	if (nModeImageSize == MODE_IMAGE_SIZE_320X240)
	{
		m_SerialPort.WriteToPort(cByteImageSize320x240, sizeof(cByteImageSize320x240)/sizeof(BYTE));
	}
	else if (nModeImageSize == MODE_IMAGE_SIZE_640X480)
	{
		m_SerialPort.WriteToPort(cByteImageSize640x480, sizeof(cByteImageSize640x480)/sizeof(BYTE));
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CLSY201::SendSavePower(int nModePower)
{
	if (nModePower == MODE_SAVE_POWER)
	{
		m_SerialPort.WriteToPort(cByteSavePower, sizeof(cByteSavePower)/sizeof(BYTE));
	}
	else if (nModePower == MODE_EXIT_SAVE_POWER)
	{
		m_SerialPort.WriteToPort(cByteExitSavePower, sizeof(cByteExitSavePower)/sizeof(BYTE));
	}
	else
	{
		ASSERT(FALSE);
	}	
}


void CLSY201::SendBaud(UINT nBaud)
{
	
	if (nBaud == 9600)
	{
		m_SerialPort.WriteToPort(cByteBaud9600, sizeof(cByteBaud9600)/sizeof(BYTE));
	}
	else if (nBaud == 19200)
	{
		m_SerialPort.WriteToPort(cByteBaud19200, sizeof(cByteBaud19200)/sizeof(BYTE));
	}
	else if (nBaud == 38400)
	{
		m_SerialPort.WriteToPort(cByteBaud38400, sizeof(cByteBaud38400)/sizeof(BYTE));
	}
	else if (nBaud == 57600)
	{
		m_SerialPort.WriteToPort(cByteBaud57600, sizeof(cByteBaud57600)/sizeof(BYTE));
	}
	else if (nBaud == 115200)
	{
		m_SerialPort.WriteToPort(cByteBaud115200, sizeof(cByteBaud115200)/sizeof(BYTE));
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CLSY201::AddSerData(BYTE *pData, WORD wLen)
{
	ASSERT(wLen == 1);

	EnterCriticalSection(&m_csBuf);
	m_ArrBuf.Add(*pData);
	LeaveCriticalSection(&m_csBuf);

//	m_ArrBufDebug.Add(*pData);
}



int CLSY201::StartProcessThread()
{

	int nRet = 0;

	
	m_bIsWritingBuf = FALSE;
	m_bFirstFrame = FALSE;

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

	pProcessThread = AfxBeginThread(RecvBufProc, this, THREAD_PRIORITY_NORMAL);

	if (pProcessThread == NULL)
	{
		nRet = 1;
		goto ERROR_DAQPROC;
	}


	return nRet;

ERROR_DAQPROC:
	return nRet;
}

int CLSY201::StopProcessThread()
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

				::PostMessage(m_hParentWnd, UM_PROCESS_THREAD, (WPARAM)0, (LPARAM)THREAD_FORCE_EXIT);
			}
			else
			{
				::PostMessage(m_hParentWnd, UM_PROCESS_THREAD, (WPARAM)0, (LPARAM)THREAD_NORMAL_EXIT);
			}
			
			break;
		}
	}

	pProcessThread = NULL;
	return nRet;
}

UINT CLSY201::RecvBufProc(LPVOID lParam)
{
	UINT nRet = 0;
	CLSY201 *pLSY201;
	BOOL  bExit;

	pLSY201 = (CLSY201 *)lParam;


	pLSY201->bRun = TRUE;
	
	TRACE("RecvBufProc is started\n");
	
	bExit = FALSE;

	while(!bExit)
	{

		if (WaitForSingleObject(pLSY201->hEventShut, 0) == WAIT_OBJECT_0)
		{
			bExit = TRUE;
			nRet = 2;			// manual terminate
			break;
		}

		//ִ����ѯ
		pLSY201->OnRevSerData();

		Sleep(TIME_OUT_PROCESS);
	} // end while


	pLSY201->bRun = FALSE;

	TRACE("RecvBufProc is ended with code %d\n", nRet);

	AfxEndThread(nRet);

	return nRet;
}


void CLSY201::OnRevSerData()
{

	//��������Ҫ�Ľ���ֵ
	//1.ͼƬ�ĳ���
	//2.ͼƬ������

	int nCmd = CMD_INVALID;
	nCmd = QueryCmd();

	EnterCriticalSection(&m_csBuf);

	if (nCmd != CMD_INVALID)
	{
		CString strMsg;
		strMsg.Format("CMD = %d\r\n", nCmd);

// 		if (nCmd == CMD_STOP_PHOTO || 
// 			nCmd == CMD_READ_BUF_512 ||
// 			nCmd == CMD_READ_BUF_TOTAL)
// 		{
// 			m_pDlg->ReportInfo(strMsg);
// 		}
		
		TRACE(strMsg);

		if (nCmd == CMD_RESET ||
			nCmd == CMD_PHOTO ||
			nCmd == CMD_STOP_PHOTO ||
			nCmd == CMD_READ_LENGTH ||
			nCmd == CMD_ZIP_RATE ||
			nCmd == CMD_SAVE_POWER ||
			nCmd == CMD_EXIT_SAVE_POWER ||
			nCmd == CMD_MODIFY_BAUD ||
			nCmd == CMD_MODIFY_IMAGE_SIZE)
		{
			int nLength = m_ArrBuf.GetSize(); 

			if (nLength <= 0)
			{
				goto ret;
			}

			//����֡ͷ��־0x76
			BOOL bFind = FALSE;
			int nIndex = -1;
			for (int ii = 0; ii < nLength; ++ii)
			{
				if (m_ArrBuf.GetAt(ii) == 0x76)
				{
					nIndex = ii;
					bFind = TRUE;
					break;
				}
			}
			
			if (bFind)
			{
				if (nIndex > 0)
				{
					m_ArrBuf.RemoveAt(0, nIndex);
				}
			}
			else //ȫ���������
			{
				//ɾ�������Ч����
				//��������,���ٴ���!
				m_ArrBuf.RemoveAll();
				goto ret;
			}

			//���¼���
			nLength = m_ArrBuf.GetSize(); 

			switch (nCmd)
			{
			case CMD_RESET:
				//���ٵķ���ֵ��4+nSizeRetOnPower���ֽ�
				if (nLength < 4 + nSizeRetOnPower)
				{
					goto ret;
				}
				
				//ȡǰ�����ֽ�,�Ƚ���ʲô�����ķ���ֵ
				if (m_ArrBuf.GetAt(0) == cByteRetReset[0] &&
					m_ArrBuf.GetAt(1) == cByteRetReset[1] &&
					m_ArrBuf.GetAt(2) == cByteRetReset[2] &&
					m_ArrBuf.GetAt(3) == cByteRetReset[3])
				{
				
					//�ж���󼸸��ֽ�49 6E 69 74 20 65 6E 64 0D 0A �Ϳ���
					BOOL bFind = FALSE;
					int nIndex = -1;
					int ii = 0;
					for (ii = 4; ii < nLength-1; ++ii)
					{
						if (m_ArrBuf.GetAt(ii) == 0x49 && 
							m_ArrBuf.GetAt(ii+1) == 0x6E)
						{
							nIndex = ii-4;
							bFind = TRUE;
							break;
						}
					}
					
					if (bFind)
					{
						if (nIndex > 0)
						{
							m_ArrBuf.RemoveAt(4, nIndex);
						}
					}
					else //ȫ���������
					{
						//ɾ�������Ч����
						//��������,���ٴ���!
						m_ArrBuf.RemoveAt(4, nLength-4);
						goto ret;
					}
					
					//���¼���
					nLength = m_ArrBuf.GetSize(); 
					if (nLength < nSizeRetOnPower)
					{
						goto ret;
					}
					
					//��������
					DeleteCmd();

					ii = 0;
					//�ж��Ƿ��ϵ�
					for (ii = 0; ii < nSizeRetOnPower; ++ii)
					{
						if (m_ArrBuf.GetAt(4+ii) != cByteRetOnPower[ii])
						{
							break;
						}
					}
					
					if (ii == nSizeRetOnPower)
					{
						m_pDlg->ReportInfo("��λ�ɹ�!�յ�Init end!");
						m_ArrBuf.RemoveAt(0, 4+nSizeRetOnPower);
					}
					else
					{
						m_pDlg->ReportInfo("��λ�յ�δ֪�ַ�,��ս��ջ�����!");
						m_ArrBuf.RemoveAll();
					}
					goto ret;
				}
				else
				{

				}
				break;
			case CMD_PHOTO:
				//���˸�λ��������,��������5������ֵ
				if (nLength < 5)
				{
					goto ret;
				}
				
				
				if (m_ArrBuf.GetAt(0) == cByteRetPhoto[0] &&
					m_ArrBuf.GetAt(1) == cByteRetPhoto[1] &&
					m_ArrBuf.GetAt(2) == cByteRetPhoto[2] &&
					m_ArrBuf.GetAt(3) == cByteRetPhoto[3] &&
					m_ArrBuf.GetAt(4) == cByteRetPhoto[4])
				{
					//��������
					DeleteCmd();

//					AfxMessageBox("�յ����ճɹ���������");

					m_ArrBuf.RemoveAt(0, 5);
					m_pDlg->ReportInfo("���ճɹ�!");
					m_pDlg->DoSendReadLength();
				}		
				break;
			case CMD_STOP_PHOTO:
				if (nLength < 5)
				{
					goto ret;
				}
				
				
				if (m_ArrBuf.GetAt(0) == cByteRetPhoto[0] &&
					m_ArrBuf.GetAt(1) == cByteRetPhoto[1] &&
					m_ArrBuf.GetAt(2) == cByteRetPhoto[2] &&
					m_ArrBuf.GetAt(3) == cByteRetPhoto[3] &&
					m_ArrBuf.GetAt(4) == cByteRetPhoto[4])
				{
				
//					AfxMessageBox("�յ�ֹͣ���ճɹ���������");

					m_pDlg->ReportInfo("ɾ��ֹͣ��������!");

					//��������
					DeleteCmd();
					
					m_ArrBuf.RemoveAt(0, 5);
					m_pDlg->ReportInfo("ֹͣ���ճɹ�!");
					//m_pDlg->DoNext();

					
					//��������ж��Ƿ���������ģʽ

					Sleep(100);

//					AfxMessageBox("��ʼ��һ��");
					m_pDlg->PhotoNext();
				}
				break;
			case CMD_READ_LENGTH:

				
				if (m_ArrBuf.GetAt(0) == cByteRetReadLength[0] &&
					m_ArrBuf.GetAt(1) == cByteRetReadLength[1] &&
					m_ArrBuf.GetAt(2) == cByteRetReadLength[2] &&
					m_ArrBuf.GetAt(3) == cByteRetReadLength[3] &&
					m_ArrBuf.GetAt(4) == cByteRetReadLength[4])
				{
					if (nLength < 9)
					{
						goto ret;
					}
					
					DWORD dwLength = m_ArrBuf.GetAt(5) * 0x1000000 + 
						m_ArrBuf.GetAt(6) * 0x10000 +
						m_ArrBuf.GetAt(7) * 0x100 +
						m_ArrBuf.GetAt(8);
					
					ASSERT(dwLength > 0);
					if (dwLength > 0)
					{
						
						m_dwWriteLen = 0;
						m_dwTotalLen = dwLength;
						
						//��������
						DeleteCmd();

						m_pDlg->SetFileLength(dwLength);
						
						CString strInfo;
						strInfo.Format("��ȡ�ɹ�,����%ld!", dwLength);
						m_pDlg->ReportInfo(strInfo);
						
						//m_pDlg->DoNext();
						
						//�ȴ��߳�����
						//���������,���Ըĳ��߳�ֱ������,��Ϊ����������ģʽ
						//Sleep(100);
						//�ĳ��߳�ֱ������
						
						m_pDlg->IndRevJpegData();
					}
					else
					{
						m_pDlg->ReportError("��ȡjpg����Ϊ0!");
					}
					
					m_ArrBuf.RemoveAt(0, 9);
					goto ret;
				}
				break;
			case CMD_ZIP_RATE:
				if (nLength < 5)
				{
					goto ret;
				}
				if (m_ArrBuf.GetAt(0) == cByteRetZipRate[0] &&
					m_ArrBuf.GetAt(1) == cByteRetZipRate[1] &&
					m_ArrBuf.GetAt(2) == cByteRetZipRate[2] &&
					m_ArrBuf.GetAt(3) == cByteRetZipRate[3] &&
					m_ArrBuf.GetAt(4) == cByteRetZipRate[4])
				{
					//��������
					DeleteCmd();

					m_ArrBuf.RemoveAt(0, 5);
					m_pDlg->ReportInfo("����ѹ���ʳɹ�!");
					goto ret;
				}
				break;
			case CMD_SAVE_POWER:
				if (nLength < 5)
				{
					goto ret;
				}
				if (m_ArrBuf.GetAt(0) == cByteRetSavePower[0] &&
					m_ArrBuf.GetAt(1) == cByteRetSavePower[1] &&
					m_ArrBuf.GetAt(2) == cByteRetSavePower[2] &&
					m_ArrBuf.GetAt(3) == cByteRetSavePower[3] &&
					m_ArrBuf.GetAt(4) == cByteRetSavePower[4])
				{
					//��������
					DeleteCmd();

					m_ArrBuf.RemoveAt(0, 5);
					m_pDlg->ReportInfo("�ı�ʡ��ģʽ�ɹ�!");
					goto ret;
				}
				break;
			case CMD_EXIT_SAVE_POWER:
				if (nLength < 5)
				{
					goto ret;
				}
				if (m_ArrBuf.GetAt(0) == cByteRetSavePower[0] &&
					m_ArrBuf.GetAt(1) == cByteRetSavePower[1] &&
					m_ArrBuf.GetAt(2) == cByteRetSavePower[2] &&
					m_ArrBuf.GetAt(3) == cByteRetSavePower[3] &&
					m_ArrBuf.GetAt(4) == cByteRetSavePower[4])
				{
					//��������
					DeleteCmd();

					m_ArrBuf.RemoveAt(0, 5);
					m_pDlg->ReportInfo("�ı�ʡ��ģʽ�ɹ�!");
					goto ret;
				}
				break;
			case CMD_MODIFY_BAUD:
				break;
			case CMD_MODIFY_IMAGE_SIZE:
				if (nLength < 5)
				{
					goto ret;
				}
				if (m_ArrBuf.GetAt(0) == cByteRetImageSize320x240[0] &&
					m_ArrBuf.GetAt(1) == cByteRetImageSize320x240[1] &&
					m_ArrBuf.GetAt(2) == cByteRetImageSize320x240[2] &&
					m_ArrBuf.GetAt(3) == cByteRetImageSize320x240[3] &&
					m_ArrBuf.GetAt(4) == cByteRetImageSize320x240[4])
				{
					//��������
					DeleteCmd();

					m_ArrBuf.RemoveAt(0, 5);
					m_pDlg->ReportInfo("����ͼ���С�ɹ�!");
					goto ret;
				}
				break;
			default:
				ASSERT(FALSE);
			}	

			goto ret;
		} //��������ڴ˷���


		if (nCmd == CMD_READ_BUF_512)
		{
			//���¼���
			UINT nLength = m_ArrBuf.GetSize(); 
			if (nLength <= 0)
			{
				goto ret;
			}

			if (m_bRecvFrameHead == FALSE)
			{
				//����֡ͷ��־0x76
				BOOL bFind = FALSE;
				int nIndex = -1;
				for (UINT ii = 0; ii < nLength; ++ii)
				{
					if (m_ArrBuf.GetAt(ii) == 0x76)
					{
						nIndex = ii;
						bFind = TRUE;
						break;
					}
				}
				
				if (bFind)
				{
					if (nIndex > 0)
					{
						m_ArrBuf.RemoveAt(0, nIndex);
					}
				}
				else //ȫ���������
				{
					//ɾ�������Ч����
					//��������,���ٴ���!
					m_ArrBuf.RemoveAll();
					goto ret;
				}
				
				//���¼���
				nLength = m_ArrBuf.GetSize(); 
				
				if (nLength < 5)
				{
					goto ret;
				}
				
				if (m_ArrBuf.GetAt(0) == 0x76 &&
					m_ArrBuf.GetAt(1) == g_nSerialNum &&
					m_ArrBuf.GetAt(2) == 0x32 &&
					m_ArrBuf.GetAt(3) == 0x00 && 
					m_ArrBuf.GetAt(4) == 0x00)
				{

					m_ArrBuf.RemoveAt(0, 5);
					
					//����֡ͷ�� 
					m_bRecvFrameHead = TRUE;

					if (m_dwWriteLen == 0)
					{
						//��ʼ�����ļ���
						m_bIsWritingBuf = TRUE;
						m_bFirstFrame = TRUE;

						m_pDlg->CreateWriteFile();
						m_pDlg->ReportInfo("�����ļ�!");
	
					}
					else
					{
						m_pDlg->ReportInfo("����512֡ͷ!");
					}

					m_bRecvFrameHead = TRUE;
				}
				else
				{
					//ֻɾ����һ��0x76,��һ�λ��Զ�������һ��0x76
					//ֻ������,����Ż����׳
					m_ArrBuf.RemoveAt(0, 1);
					goto ret;
				}

			}


			if (m_bRecvFrameHead)
			{
				if (m_bFindFrameEnd) 
				{
					BOOL bRecvFrameEnd = FALSE; //�յ���ǰ֡��֡β

					//���¼��㳤��
					UINT nLength = m_ArrBuf.GetSize();

					//����֡β��־0x76
					BOOL bFind = FALSE;
					int nIndex = -1;
					for (UINT ii = 0; ii < nLength; ++ii)
					{
						if (m_ArrBuf.GetAt(ii) == 0x76)
						{
							nIndex = ii;
							bFind = TRUE;
							break;
						}
					}
					
					if (bFind)
					{
						if (nIndex > 0)
						{
							m_ArrBuf.RemoveAt(0, nIndex);
						}
					}
					else //ȫ���������
					{
						//ɾ�������Ч����
						//��������,���ٴ���!
						m_ArrBuf.RemoveAll();
						goto ret;
					}

					//���¼���
					nLength = m_ArrBuf.GetSize(); 
					
					if (nLength < 5)
					{
						goto ret;
					}
					
					if (m_ArrBuf.GetAt(0) == 0x76 &&
						m_ArrBuf.GetAt(1) == g_nSerialNum &&
						m_ArrBuf.GetAt(2) == 0x32 &&
						m_ArrBuf.GetAt(3) == 0x00 && 
						m_ArrBuf.GetAt(4) == 0x00)
					{
						
						m_ArrBuf.RemoveAt(0, 5);		
						bRecvFrameEnd = TRUE;

						//����������һ֡
						ASSERT(m_dwWriteLen > 0);
						if (m_dwWriteLen < m_dwTotalLen &&
							m_dwWriteLen > 0)
						{
							DeleteCmd();
							m_pDlg->ReportInfo("�յ�512֡β");
							m_pDlg->IndRevJpegData();
						}
						else if (m_dwWriteLen == m_dwTotalLen)
						{

							m_pDlg->ReportInfo("������ɾ��CMD_READ_BUF_512");
							DeleteCmd();

							m_pDlg->ReportInfo("�յ�ȫ��֡�������֡β");

							Sleep(50);

							m_pDlg->PhotoStop();
						}
						else
						{
							ASSERT(FALSE);
						}
						
					}
					else
					{
						//ֻɾ����һ��0x76,��һ�λ��Զ�������һ��0x76
						//ֻ������,����Ż����׳
						m_ArrBuf.RemoveAt(0, 1);
						goto ret;
					}

					if (bRecvFrameEnd)
					{
						
						m_bRecvFrameHead = FALSE;
						m_bFindFrameEnd = FALSE;
					}

					goto ret;
				}

				
				//���¼��㳤��
				UINT nLength = m_ArrBuf.GetSize();

				BYTE szBuf[BLOCK_SIZE];
				
				//һ��һ�ζ�
				if(nLength > 0)
				{
					DWORD dwLeft = m_dwTotalLen - m_dwWriteLen;

					ASSERT(dwLeft >= 0 && dwLeft <= m_dwTotalLen);

					if(dwLeft <= BLOCK_SIZE) //���һ��
					{
						if (nLength < dwLeft)
						{
							goto ret;
						}
						
						memset(szBuf, 0, BLOCK_SIZE);
						for (DWORD ii = 0; ii < dwLeft; ++ii)
						{
							szBuf[ii] = m_ArrBuf.GetAt(ii);
						}
						
						m_pDlg->WriteBufToFile(szBuf, dwLeft);
						m_ArrBuf.RemoveAt(0, dwLeft);  //ɾ��
						
						m_dwWriteLen += dwLeft;	
						
						//����֡β 
						m_bFindFrameEnd = TRUE;
					}
					else
					{
						//һ��д512�ֽڸ��ϲ�,��ʱ�ͷŻ�����

						if (nLength < BLOCK_SIZE)
						{
							goto ret;
						}

						memset(szBuf, 0, BLOCK_SIZE);
						for (DWORD ii = 0; ii < BLOCK_SIZE; ++ii)
						{
							szBuf[ii] = m_ArrBuf.GetAt(ii);
						}
						
						m_pDlg->WriteBufToFile(szBuf, BLOCK_SIZE);
						m_ArrBuf.RemoveAt(0, BLOCK_SIZE);  //ɾ��
						
						m_dwWriteLen += BLOCK_SIZE;

						//����֡β 
						m_bFindFrameEnd = TRUE;						
					}

					//nLength = m_ArrBuf.GetSize();  //���¼���
				}

				goto ret;
			}
		}


		if (nCmd == CMD_READ_BUF_TOTAL)
		{
			
			//���¼���
			UINT nLength = m_ArrBuf.GetSize(); 
			if (nLength <= 0)
			{
				goto ret;
			}

			if (m_bRecvFrameHead == FALSE)
			{

				//����֡ͷ��־0x76
				BOOL bFind = FALSE;
				int nIndex = -1;
				for (UINT ii = 0; ii < nLength; ++ii)
				{
					if (m_ArrBuf.GetAt(ii) == 0x76)
					{
						nIndex = ii;
						bFind = TRUE;
						break;
					}
				}
				
				if (bFind)
				{
					if (nIndex > 0)
					{
						m_ArrBuf.RemoveAt(0, nIndex);
					}
				}
				else //ȫ���������
				{
					//ɾ�������Ч����
					//��������,���ٴ���!
					m_ArrBuf.RemoveAll();
					goto ret;
				}
				
				//���¼���
				nLength = m_ArrBuf.GetSize(); 
				
				if (nLength < 5)
				{
					goto ret;
				}
				
				if (m_ArrBuf.GetAt(0) == 0x76 &&
					m_ArrBuf.GetAt(1) == g_nSerialNum &&
					m_ArrBuf.GetAt(2) == 0x32 &&
					m_ArrBuf.GetAt(3) == 0x00 && 
					m_ArrBuf.GetAt(4) == 0x00)
				{
					//��ʼ�����ļ���
					m_bIsWritingBuf = TRUE;
					m_bFirstFrame = TRUE;
					m_ArrBuf.RemoveAt(0, 5);
					
					m_pDlg->CreateWriteFile();
					m_pDlg->ReportInfo("�����ļ�!");
					
					m_bRecvFrameHead = TRUE;
				}
				else
				{
					//ֻɾ����һ��0x76,��һ�λ��Զ�������һ��0x76
					//ֻ������,����Ż����׳
					m_ArrBuf.RemoveAt(0, 1);
					goto ret;
				}

			}


			if (m_bRecvFrameHead)
			{
				m_pDlg->ReportInfo("m_bRecvFrameHead = TRUE");
				if (m_bFindFrameEnd) 
				{
					m_pDlg->ReportInfo("m_bFindFrameEnd = TRUE");
					BOOL bRecvFrameEnd = FALSE; //�յ���ǰ֡��֡β

					//���¼��㳤��
					UINT nLength = m_ArrBuf.GetSize();

					m_pDlg->ReportInfo("m_bFindFrameEnd = TRUE,����֡β��־0x76");

					//����֡β��־0x76
					BOOL bFind = FALSE;
					int nIndex = -1;
					for (UINT ii = 0; ii < nLength; ++ii)
					{
						if (m_ArrBuf.GetAt(ii) == 0x76)
						{
							nIndex = ii;
							bFind = TRUE;
							break;
						}
					}
					
					if (bFind)
					{
						if (nIndex > 0)
						{
							m_ArrBuf.RemoveAt(0, nIndex);
						}
					}
					else //ȫ���������
					{
						//ɾ�������Ч����
						//��������,���ٴ���!
						m_ArrBuf.RemoveAll();
						goto ret;
					}

					//���¼���
					nLength = m_ArrBuf.GetSize(); 
					
					if (nLength < 5)
					{
						goto ret;
					}
					
					m_pDlg->ReportInfo("�ȴ�֡β76 00 32 00 00");
					if (m_ArrBuf.GetAt(0) == 0x76 &&
						m_ArrBuf.GetAt(1) == g_nSerialNum &&
						m_ArrBuf.GetAt(2) == 0x32 &&
						m_ArrBuf.GetAt(3) == 0x00 && 
						m_ArrBuf.GetAt(4) == 0x00)
					{
////						AfxMessageBox("�յ�֡β76 00 32 00 00");
						m_pDlg->ReportInfo("�ɹ�:�ȴ�֡β76 00 32 00 00");

						m_ArrBuf.RemoveAt(0, 5);		
						bRecvFrameEnd = TRUE;

						
						m_bRecvFrameHead = FALSE;
						m_bFindFrameEnd = FALSE;

						DeleteCmd();
						m_pDlg->ReportInfo("������ɾ��CMD_READ_TOTAL");

						m_pDlg->ReportInfo("�յ��ļ����������־");

						Sleep(50);

						m_pDlg->PhotoStop();
					}
					else
					{
						//ֻɾ����һ��0x76,��һ�λ��Զ�������һ��0x76
						//ֻ������,����Ż����׳
						m_ArrBuf.RemoveAt(0, 1);
						goto ret;
					}

					if (bRecvFrameEnd)
					{
						m_pDlg->ReportInfo("����m_bRecvFrameHead��m_bFindFrameEndΪFALSE");
						m_bRecvFrameHead = FALSE;
						m_bFindFrameEnd = FALSE;
					}

					goto ret;
				}
				else
				{
					m_pDlg->ReportInfo("m_bFindFrameEnd = FALSE");
				}
				
				//���¼��㳤��
				UINT nLength = m_ArrBuf.GetSize();


				BYTE szBuf[BLOCK_SIZE];
				

				while(nLength > 0)
				{
					

					DWORD dwLeft = m_dwTotalLen - m_dwWriteLen;

					ASSERT(dwLeft >= 0 && dwLeft <= m_dwTotalLen);

					if(dwLeft >= 0 && dwLeft <= BLOCK_SIZE) //���һ��
					{
						m_pDlg->ReportInfo("���һ��");
						if (dwLeft == 0 || nLength < dwLeft)
						{
							goto ret;
						}
						
						
						memset(szBuf, 0, BLOCK_SIZE);
						for (DWORD ii = 0; ii < dwLeft; ++ii)
						{
							szBuf[ii] = m_ArrBuf.GetAt(ii);
						}
						
						m_pDlg->WriteBufToFile(szBuf, dwLeft);
						m_ArrBuf.RemoveAt(0, dwLeft);  //ɾ��
						
						m_dwWriteLen += dwLeft;	
						
						m_pDlg->ReportInfo("����m_bFindFrameEnd = TRUE");
						//����֡β 
						m_bFindFrameEnd = TRUE;
					}
					else
					{
						//һ��д512�ֽڸ��ϲ�,��ʱ�ͷŻ�����

						if (nLength < BLOCK_SIZE)
						{
							goto ret;
						}

						m_pDlg->ReportInfo("һ��д512�ֽڸ��ϲ�,��ʱ�ͷŻ�����");

						memset(szBuf, 0, BLOCK_SIZE);
						for (DWORD ii = 0; ii < BLOCK_SIZE; ++ii)
						{
							szBuf[ii] = m_ArrBuf.GetAt(ii);
						}
						
						m_pDlg->WriteBufToFile(szBuf, BLOCK_SIZE);
						m_ArrBuf.RemoveAt(0, BLOCK_SIZE);  //ɾ��
						
						m_dwWriteLen += BLOCK_SIZE;
						
					}

					nLength = m_ArrBuf.GetSize();  //���¼���
				}

				goto ret;
			}
		}
		


	}
	else
	{
		int nLength = m_ArrBuf.GetSize(); 
		
		if (nLength <= 0)
		{
			goto ret;
		}
		
		if (nLength < nSizeRetOnPower)
		{
			goto ret;
		}

		//�ж���󼸸��ֽ�49 6E 69 74 20 65 6E 64 0D 0A �Ϳ���
		BOOL bFind = FALSE;
		int nIndex = -1;
		int ii = 0;
		for (ii = 0; ii < nLength-1; ++ii)
		{
			if (m_ArrBuf.GetAt(ii) == 0x49 && 
				m_ArrBuf.GetAt(ii+1) == 0x6E)
			{
				nIndex = ii;
				bFind = TRUE;
				break;
			}
		}
		
		if (bFind)
		{
			if (nIndex > 0)
			{
				m_ArrBuf.RemoveAt(0, nIndex);
			}
		}
		else //ȫ���������
		{
			//ɾ�������Ч����
			//��������,���ٴ���!
			m_ArrBuf.RemoveAll();
			goto ret;
		}

		//���¼���
		nLength = m_ArrBuf.GetSize(); 
		if (nLength < nSizeRetOnPower)
		{
			goto ret;
		}

		//�ж��Ƿ��ϵ�
		for (ii = 0; ii < nSizeRetOnPower; ++ii)
		{
			if (m_ArrBuf.GetAt(ii) != cByteRetOnPower[ii])
			{
				break;
			}
		}

		if (ii == nSizeRetOnPower)
		{
			m_pDlg->ReportInfo("�ϵ�ɹ�!�յ�Init end!");
			m_ArrBuf.RemoveAt(0, nSizeRetOnPower);
		}
		else
		{
		//	m_pDlg->ReportInfo("�ϵ��յ�δ֪�ַ�,��ս��ջ�����!");
			m_ArrBuf.RemoveAll();
		}

	}

ret:
	LeaveCriticalSection(&m_csBuf);
	return;
}

void CLSY201::Reset()
{
	EnterCriticalSection(&m_csBuf);
	m_ArrBuf.RemoveAll();
	LeaveCriticalSection(&m_csBuf);
	m_dwWriteLen = 0;
	m_dwTotalLen = 0;
	m_bWaitLastFrame = FALSE;
	m_bIsWritingBuf = FALSE;
}

void CLSY201::SetSerialNum()
{

	cByteReset[ID_SERIAL_NUM] = g_nSerialNum;
	cBytePhoto[ID_SERIAL_NUM] = g_nSerialNum;
	cByteReadLength[ID_SERIAL_NUM] = g_nSerialNum;
	cByteReadBuf[ID_SERIAL_NUM] = g_nSerialNum;
	cByteStopPhoto[ID_SERIAL_NUM] = g_nSerialNum;
	cByteZipRate[ID_SERIAL_NUM] = g_nSerialNum;
	cByteImageSize320x240[ID_SERIAL_NUM] = g_nSerialNum;
	cByteImageSize640x480[ID_SERIAL_NUM] = g_nSerialNum;
	cByteSavePower[ID_SERIAL_NUM] = g_nSerialNum;
	cByteExitSavePower[ID_SERIAL_NUM] = g_nSerialNum;
	cByteBaud9600[ID_SERIAL_NUM] = g_nSerialNum;
	cByteBaud19200[ID_SERIAL_NUM] = g_nSerialNum;
	cByteBaud38400[ID_SERIAL_NUM] = g_nSerialNum;
	cByteBaud57600[ID_SERIAL_NUM] = g_nSerialNum;
	cByteBaud115200[ID_SERIAL_NUM] = g_nSerialNum;


	cByteRetReset[ID_SERIAL_NUM] = g_nSerialNum;
	cByteRetPhoto[ID_SERIAL_NUM] = g_nSerialNum;
	cByteRetReadLength[ID_SERIAL_NUM] = g_nSerialNum;
	cByteRetZipRate[ID_SERIAL_NUM] = g_nSerialNum;
	cByteRetSavePower[ID_SERIAL_NUM] = g_nSerialNum;
	cByteRetImageSize320x240[ID_SERIAL_NUM] = g_nSerialNum;
}

//��Ϣ����,������Ϣ���п����ط�
void CLSY201::AddCmdToQueue(int nCmd)
{
	ASSERT(nCmd != CMD_INVALID);

	EnterCriticalSection(&m_csCmdQueue);
	m_ArrCmdQueue.Add(nCmd);
	LeaveCriticalSection(&m_csCmdQueue);

}

//����ͷ
int  CLSY201::QueryCmd()
{
	int nRet = CMD_INVALID;

	EnterCriticalSection(&m_csCmdQueue);
	if (m_ArrCmdQueue.GetSize() < 1)
	{
		nRet = CMD_INVALID;
	}
	else
	{	
		nRet = m_ArrCmdQueue.GetAt(0);
	}
	LeaveCriticalSection(&m_csCmdQueue);

	return nRet;
}

void  CLSY201::DeleteAllCmd()
{
	EnterCriticalSection(&m_csCmdQueue);
	
	m_ArrCmdQueue.RemoveAll();

	LeaveCriticalSection(&m_csCmdQueue);
}

void  CLSY201::DeleteCmd()
{
	EnterCriticalSection(&m_csCmdQueue);
	if (m_ArrCmdQueue.GetSize() > 0)
	{	
		m_ArrCmdQueue.RemoveAt(0);
	}
	LeaveCriticalSection(&m_csCmdQueue);
}

BOOL  CLSY201::IsCmdEmpty()
{
	BOOL bEmpty;
	EnterCriticalSection(&m_csCmdQueue);
	bEmpty = (m_ArrCmdQueue.GetSize() == 0);
	if (m_ArrCmdQueue.GetSize() > 0)
	{	
		m_ArrCmdQueue.RemoveAt(0);
	}
	LeaveCriticalSection(&m_csCmdQueue);
	return bEmpty;
}

