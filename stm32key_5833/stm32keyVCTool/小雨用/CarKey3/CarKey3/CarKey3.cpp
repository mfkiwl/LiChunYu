
// CarKey3.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgNEC_CHIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarKey3App

BEGIN_MESSAGE_MAP(CCarKey3App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCarKey3App ����

CCarKey3App::CCarKey3App()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCarKey3App ����

CCarKey3App theApp;


// CCarKey3App ��ʼ��

BOOL CCarKey3App::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	HANDLE hObject = ::CreateMutex(NULL,FALSE, _T("wflcystm32keyTool20190425")); 
	if(GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		CloseHandle(hObject); 
		CString str,str1;
		str.LoadString(IDS_OneOpen);
		str1.LoadString(IDS_Warring);
		MessageBox(NULL,str, str1, MB_ICONERROR|MB_OK); 
		return FALSE; 
	}
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	CDlgNEC_CHIP dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
void CCarKey3App::WaitMS(int ms)
{
	MSG msg; //����һ��MSG���͵ı���
	while (ms != 0)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) //��ȡ��Ϣ���Ѹ���Ϣ����Ϣ�������Ƴ�(��ֹ�ظ���Ӧ)��
		{
			::DispatchMessage(&msg); //����Ϣ�ƽ������̺���
			::TranslateMessage(&msg);//������Ϣ �ں��ʵĻ������char��Ϣ
		}
		Sleep(1);
		ms--;
	}
}

float CCarKey3App::m_fHexVer=0;

WORD CCarKey3App::LANGID;

CString CCarKey3App::strLogFormat=_T("%-30s%-20s%s\r\n");

byte CCarKey3App::HPassword[8];

byte CCarKey3App::PassSSID[4];

bool CCarKey3App::m_bPass=false;

float CCarKey3App::m_fSoftVer=30.02f;

CString CCarKey3App::RunPath;
CWithServer CCarKey3App::withServer;
CHIDWork CCarKey3App::TheHIDDevice;
