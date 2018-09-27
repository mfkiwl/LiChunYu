
// CarKey3Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CarKey3.h"
#include "CarKey3Dlg.h"
#include "afxdialogex.h"
#include "DlgNEC_CHIP.h"
#include ".\..\..\CarKeyCommon\CommFunc.h"
#include "DlgEIS.h"
#include "DlgESL.h"
#include "DlgPSW.h"
#include <afxwin.h>
#include "UpdateSoft.h"
#include "DlgECU.h"
#include "DlgISM_GBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCarKey3Dlg �Ի���




CCarKey3Dlg::CCarKey3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCarKey3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bShow=true;
}

void CCarKey3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCarKey3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CCarKey3Dlg::OnBnPSW)
	ON_COMMAND(ID_LANGUAGE_32775, &CCarKey3Dlg::OnLanguage32775)
	ON_COMMAND(ID_LANGUAGE_ENGLISH, &CCarKey3Dlg::OnLanguageEnglish)
	ON_BN_CLICKED(IDC_BUTTON2, &CCarKey3Dlg::OnBnEIS)
	ON_BN_CLICKED(IDC_BUTTON1, &CCarKey3Dlg::OnBnKey)
	ON_BN_CLICKED(IDC_BUTTON3, &CCarKey3Dlg::OnBnESL)
	ON_BN_CLICKED(IDC_BUTTON6, &CCarKey3Dlg::OnBnECU)
	ON_BN_CLICKED(IDC_BUTTON5, &CCarKey3Dlg::OnBnIG)
END_MESSAGE_MAP()


// CCarKey3Dlg ��Ϣ�������

BOOL CCarKey3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);

	int strPartDim[1]= {-1}; //�ָ�����
	m_StatusBar.SetParts(1, strPartDim);
	//����״̬���ı�
	m_StatusBar.SetText(_T(""), 0, 0);
	//��������״̬���м���ͼ��
	m_StatusBar.SetIcon(1, SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE));//Ϊ�ڶ��������мӵ�ͼ��
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_btnMKEY.LoadBitmaps(IDB_BITMAP9, IDB_BITMAP10);
	m_btnMKEY.SubclassDlgItem(IDC_BUTTON1, this);
	m_btnMKEY.SizeToContent();

	m_btnEIS.LoadBitmaps(IDB_BITMAP3, IDB_BITMAP4);
	m_btnEIS.SubclassDlgItem(IDC_BUTTON2, this);
	m_btnEIS.SizeToContent();

	m_btnESL.LoadBitmaps(IDB_BITMAP5, IDB_BITMAP6);
	m_btnESL.SubclassDlgItem(IDC_BUTTON3, this);
	m_btnESL.SizeToContent();

	m_btnNEC_CHIP.LoadBitmaps(IDB_BITMAP12, IDB_BITMAP11);
	m_btnNEC_CHIP.SubclassDlgItem(IDC_BUTTON4, this);
	m_btnNEC_CHIP.SizeToContent();

	m_btnNEC_KEYLESS.LoadBitmaps(IDB_BITMAP7, IDB_BITMAP8);
	m_btnNEC_KEYLESS.SubclassDlgItem(IDC_BUTTON5, this);
	m_btnNEC_KEYLESS.SizeToContent();

	m_btnNEC_ESL.LoadBitmaps(IDB_BITMAP1, IDB_BITMAP2);
	m_btnNEC_ESL.SubclassDlgItem(IDC_BUTTON6, this);
	m_btnNEC_ESL.SizeToContent();


	CString   path=AfxGetApp()->m_pszHelpFilePath;   
	CString   str=AfxGetApp()->m_pszExeName;   
	path=path.Left(path.GetLength()-str.GetLength()-4); 
	CCarKey3App::RunPath=path;
	path+=_T("Language.ini");
	DWORD dwVersion = GetVersion();

	// Get the Windows version.

	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
	LANGID id=GetSystemDefaultLangID();
	if(dwWindowsMajorVersion==6)
	{
		if(id==0x0804)//����
		{
			CCarKey3App::LANGID=MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
			SetThreadUILanguage(CCarKey3App::LANGID);
			//GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_BUTTON6)->ShowWindow(SW_HIDE);
		}
		else
		{
			CCarKey3App::LANGID=MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
			SetThreadUILanguage(CCarKey3App::LANGID);
		}
	}
	else
	{
		if(id==0x0804)//����
		{
			CCarKey3App::LANGID=MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
			SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT));
			//GetDlgItem(IDC_BUTTON5)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_BUTTON6)->ShowWindow(SW_HIDE);
		}
		else
		{
			CCarKey3App::LANGID=MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
			SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
		}
	}
// 	GetPrivateProfileString(_T("Form"),_T("Language"), _T(""), str.GetBuffer(MAX_PATH), MAX_PATH ,path);
// 	if(str==_T("Chinese"))
// 	{
// 		CCarKey3App::LANGID=MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
// 		SetThreadUILanguage(CCarKey3App::LANGID);
// 		GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
// 		GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_UNCHECKED);
// 	}
// 	else if(str==_T("English"))
// 	{
// 		CCarKey3App::LANGID=MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
// 		SetThreadUILanguage(CCarKey3App::LANGID);
// 		GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_UNCHECKED);
// 		GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_CHECKED);
// 	}
// 	else
// 	{
// 		WritePrivateProfileString(_T("Form"),_T("Language"), _T("Chinese"),path); 
// 		CCarKey3App::LANGID=MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
// 		SetThreadUILanguage(CCarKey3App::LANGID);
// 		GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
// 		GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_UNCHECKED);
// 	}
	LoadString();
	CCarKey3App::TheHIDDevice.hOwner=this->m_hWnd;
	m_hUsbEventHandle = CCarKey3App::TheHIDDevice.RegisterForUsbEvents(this->GetSafeHwnd());
	EnableWindow(FALSE);
	
	SetTimer(1,1000,NULL);//������ʱ��1,��ʱʱ����1��
	SetTimer(2,500,NULL);//������ʱ��1,��ʱʱ����1��	

	CString strTemp,strTemp1;
	strTemp.LoadString(IDS_Title);
	strTemp1.Format(strTemp,CCarKey3App::m_fSoftVer);
	SetWindowText(strTemp1);
	strTemp.LoadString(IDS_Loading);
	m_StatusBar.SetText(strTemp, 0, 0);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CCarKey3Dlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (message == 0x0219)
	{
		switch (wParam)
		{
		case 0x8000:
			if (!CCarKey3App::TheHIDDevice.MyDeviceDetected)
			{
				CCarKey3App::TheHIDDevice.FindDevice(); 
			}
			HIDUI();
			break;
		case 0x8004:
			// 			try
			// 			{
			CCarKey3App::TheHIDDevice.Close();
			// 			}
			// 			catch (CException* e)
			// 			{
			// 				throw _T("USB �Ƴ�����!!");
			// 			}
			HIDUI();
			break;
		default:
			break;
		}
	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}

void CCarKey3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCarKey3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCarKey3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCarKey3Dlg::OnLanguage32775()
{
	CCarKey3App::LANGID=MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED);
	SetThreadUILanguage(CCarKey3App::LANGID);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_UNCHECKED);

	WritePrivateProfileString(_T("Form"),_T("Language"), _T("Chinese"),CCarKey3App::RunPath+_T("Language.ini")); 

	LoadString();
}

void CCarKey3Dlg::OnLanguageEnglish()
{
	CCarKey3App::LANGID=MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
	SetThreadUILanguage(CCarKey3App::LANGID);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_CHECKED);

	WritePrivateProfileString(_T("Form"),_T("Language"), _T("English"),CCarKey3App::RunPath+_T("Language.ini")); 

	LoadString();
}

void CCarKey3Dlg::LoadString()
{
 	CString strTemp;
 	//m_ListBox.ResetContent();
//  	strTemp.LoadString(IDS_KeyModule);
//  	m_ListBox.AddString(strTemp);	
//  	m_ListBox.AddString(_T("W164-NEC-old"));
//  	m_ListBox.AddString(_T("W164-NEC-new"));	
//  	m_ListBox.AddString(_T("W166-NEC"));
//  	m_ListBox.AddString(_T("W204-NEC"));
//  	m_ListBox.AddString(_T("W207-NEC"));
//  	m_ListBox.AddString(_T("W212-NEC-old"));
//  	m_ListBox.AddString(_T("W212-NEC-new"));	
//  	m_ListBox.AddString(_T("W221-NEC-old"));
//  	m_ListBox.AddString(_T("W221-NEC-new"));	
//  	m_ListBox.AddString(_T("W246-NEC"));
//  	strTemp.LoadString(IDS_ELVModule);
//  	m_ListBox.AddString(strTemp);	
//  	strTemp.LoadString(IDS_ECUModule);
//  	m_ListBox.AddString(strTemp);		
//  	strTemp.LoadString(IDS_ISM7GModule);
//  	m_ListBox.AddString(strTemp);
 
//  	strTemp.LoadString(IDS_CarKey2_btStart);    //LoadString��ȡһ��ID�Ŷ�����ַ�����Դ 
//  	SetDlgItemText(IDC_BUTTON1,strTemp); 
 	if (CCarKey3App::TheHIDDevice.MyDeviceDetected)
 	{
 		CString strID=CCommFunc::byteToHexStr(CCarKey3App::TheHIDDevice.HexID,4,_T(""));
 		strTemp.LoadString(IDS_DeviceID);
 		m_StatusBar.SetText(strTemp+_T(":")+strID, 0, 0);
 	}
 	else
 	{
 		strTemp.LoadString(IDS_NoDevice);
 		m_StatusBar.SetText(strTemp, 0, 0);
 	}
}

void CCarKey3Dlg::OnTimer( UINT_PTR nIDEvent )
{
	if(nIDEvent==1)
	{
		KillTimer(1);        
		ReHIDUI();
	}
	else
	{
		KillTimer(2); 
		CCarKey3App::withServer.CheckUrl();
		if(GetFileAttributes(_T("UpdateSoft.exe"))!=0xFFFFFFFF)
		{
			HWND h = ::FindWindow( NULL, _T("UpdateSoft.exe"));
			if (h>0)
			{
				ULONG nProcessID;
				::GetWindowThreadProcessId(h, &nProcessID );
				HANDLE hProcessHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE,nProcessID);
				::TerminateProcess( hProcessHandle,4);
			}
			while(true)
			{
				if(DeleteFile(_T("UpdateSoft.exe")))
					break;
			}
		}
 		float softVer;
		CString strTemp;
		CWebRetData webRet;
 		//if(CCarKey3App::withServer.CheckSoftVer(softVer,CCarKey3App::m_fHexVer))
		if(CCarKey3App::withServer.CheckSoftVer(webRet) || webRet.retStatus==web_OK)
		{
			CStringArray strArray;
			CCarKey3App::withServer.GetRetList(webRet.strResult,_T("|"),strArray);	
			softVer=_ttof(strArray.GetAt(0));
			CCarKey3App::m_fHexVer=_ttof(strArray.GetAt(1));
			if(softVer>CCarKey3App::m_fSoftVer)
			{
				while(true)
				{
					if(!CCarKey3App::TheHIDDevice.MyDeviceDetected)
					{
						strTemp.LoadString(IDS_ServerTip24);
						MessageBox(strTemp);
					}
					else
					{
						break;
					}
				}
				strTemp.LoadString(IDS_ServerTip12);
				MessageBox(strTemp);
				CUpdateSoft up;
				up.DoModal();
				if(up.m_bOK)
				{
					HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_UPDATESOFT1), TEXT("UPDATESOFT"));
					DWORD dwSize = SizeofResource(NULL, hRsrc);
					HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
					LPVOID pBuffer = LockResource(hGlobal);
					CFile file(_T("UpdateSoft.exe"),CFile::modeCreate|CFile::modeWrite);
					file.Write(pBuffer,dwSize);		
					file.Close();
					if(GetFileAttributes(_T("Mercedes Tool.exe.up"))!=0xFFFFFFFF)
					{
						if(GetFileAttributes(_T("UpdateSoft.exe"))!=0xFFFFFFFF)
						{
							CString strTemp,strTemp1;
							strTemp.LoadString(IDS_Title);
							strTemp1.Format(strTemp,CCarKey3App::m_fSoftVer);
							strTemp1=_T("UpdateSoft.exe ")+strTemp1;
							int len;
							char* x=CCommFunc::CStringToChar(strTemp1,&len);
							WinExec(x,SW_SHOW);
							delete[] x;
						}
					}
				}
				//  			CString str,strTemp,strTemp1;
				//  			strTemp.LoadString(IDS_RenewWarring);
				//  			strTemp1.LoadString(IDS_Warring);
				//  			if (MessageBox(_T("������°汾��"), strTemp1, MB_YESNO) == IDNO)
				//  				return;
			}
		}
		ReHIDUI();
		EnableWindow(TRUE);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CCarKey3Dlg::HIDUI()
{
	CString strTemp,strTemp1;
	if (CCarKey3App::TheHIDDevice.MyDeviceDetected)
	{		
		USHORT hexver= CCommFunc::MakeShort(CCarKey3App::TheHIDDevice.HexVer[0], CCarKey3App::TheHIDDevice.HexVer[1]);
		//m_ListBox.EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		CString strID=CCommFunc::byteToHexStr(CCarKey3App::TheHIDDevice.HexID,4,_T(""));
		strTemp.LoadString(IDS_DeviceID);
		strTemp1=CCommFunc::byteToHexStr(CCarKey3App::TheHIDDevice.HexVer,2,_T("."));
		float hexVer=_ttof(strTemp1);
		strTemp1.Format(strTemp,strID,hexVer);
		m_StatusBar.SetText(strTemp1, 0, 0);
		KillTimer(1);
		if(m_bShow)
		{
						
			if(CCarKey3App::m_fHexVer!=0)
			{
				int nHexVer=hexVer*100;
				int nSerVer=CCarKey3App::m_fHexVer*100;
				if(nHexVer<nSerVer)
				{
					strTemp.LoadString(IDS_ServerTip28);
					MessageBox(strTemp);
					CUpdateSoft up;
					up.m_nUpType=1;
					up.DoModal();
				}
			}
		}
	}
	else
	{
		//m_ListBox.EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		strTemp.LoadString(IDS_NoDevice);
		m_StatusBar.SetText(strTemp, 0, 0);
		SetTimer(1,1000,NULL);//������ʱ��1,��ʱʱ����1��
	}
}

void CCarKey3Dlg::ReHIDUI()
{
	if (!CCarKey3App::TheHIDDevice.MyDeviceDetected)
	{
		CCarKey3App::TheHIDDevice.FindDevice(); 
	}
	HIDUI();
}

void CCarKey3Dlg::OnBnPSW()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bShow=false;
	this->ShowWindow(SW_HIDE);
	CDlgPSW dlgEIS;
	dlgEIS.DoModal();
	this->ShowWindow(SW_SHOW);
	m_bShow=true;
	ReHIDUI();
}

void CCarKey3Dlg::OnBnEIS()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bShow=false;
 	this->ShowWindow(SW_HIDE);
 	CDlgEIS dlgEIS;
 	dlgEIS.DoModal();
 	this->ShowWindow(SW_SHOW);
	m_bShow=true;
	ReHIDUI();
}

void CCarKey3Dlg::OnBnKey()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bShow=false;
	this->ShowWindow(SW_HIDE);
	CDlgNEC_CHIP dlgNEC;
	dlgNEC.m_pMainDlg=this;
	dlgNEC.DoModal();
	this->ShowWindow(SW_SHOW);
	m_bShow=true;
	ReHIDUI();
}


void CCarKey3Dlg::OnBnESL()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bShow=false;
	this->ShowWindow(SW_HIDE);
	CDlgESL dlgESL;
	dlgESL.m_pMainDlg=this;
	dlgESL.DoModal();
	this->ShowWindow(SW_SHOW);
	m_bShow=true;
	ReHIDUI();
}

void CCarKey3Dlg::OnBnECU()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bShow=false;
	this->ShowWindow(SW_HIDE);
	CDlgECU dlgECU;
	dlgECU.m_pMainDlg=this;
	dlgECU.DoModal();
	this->ShowWindow(SW_SHOW);
	m_bShow=true;
	ReHIDUI();
}


void CCarKey3Dlg::OnBnIG()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bShow=false;
	this->ShowWindow(SW_HIDE);
	CDlgISM_GBox dlgIG;
	dlgIG.m_pMainDlg=this;
	dlgIG.DoModal();
	this->ShowWindow(SW_SHOW);
	m_bShow=true;
	ReHIDUI();
}
