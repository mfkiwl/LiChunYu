#pragma once


// CDlgISM �Ի���

class CDlgISM : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgISM)

public:
	CDlgISM(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgISM();

// �Ի�������
	enum { IDD = IDD_DLG_ESL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
