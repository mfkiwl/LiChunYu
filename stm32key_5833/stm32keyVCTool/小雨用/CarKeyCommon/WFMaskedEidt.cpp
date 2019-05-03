// WFMaskedEidt.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WFMaskedEidt.h"


// CWFMaskedEidt

IMPLEMENT_DYNAMIC(CWFMaskedEidt, CMFCMaskedEdit)

CWFMaskedEidt::CWFMaskedEidt()
{
	bHex=false;
	m_bLoad=false;
	//m_sysColor=GetSysColor(COLOR_WINDOW);
	m_sysColor=GetSysColor(COLOR_HIGHLIGHT);
	m_bkBrush.CreateSolidBrush(RGB(0,255,0));      // ��ɫ����ɫ
	m_sysBrush.CreateSolidBrush(m_sysColor);
}

CWFMaskedEidt::CWFMaskedEidt(bool bhex)
{
	bHex=bhex;
}
CWFMaskedEidt::~CWFMaskedEidt()
{
}


BEGIN_MESSAGE_MAP(CWFMaskedEidt, CMFCMaskedEdit)
	ON_WM_CHAR()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CWFMaskedEidt ��Ϣ�������




void CWFMaskedEidt::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	if(bHex)
// 	{
// 		if((nChar>='0' && nChar <='9') || (nChar>='a' && nChar<='f') || (nChar>='A' && nChar<='F'))
// 		{
// 			CMFCMaskedEdit::OnChar(nChar, nRepCnt, nFlags);
// 		}
// 		else
// 			return;
// 	}
	CMFCMaskedEdit::OnChar(nChar, nRepCnt, nFlags);
}


HBRUSH CWFMaskedEidt::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = CMFCMaskedEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	if(m_bLoad)
	{
		CString str;
		this->GetWindowText(str);
		if(str==m_strContent)
		{
			pDC->SetBkColor(m_sysColor);
			return m_sysBrush;
		}
		else
		{
			pDC->SetBkColor(RGB(0,255,0));
			return m_bkBrush;
		}
	}
	else
	{
		pDC->SetBkColor(m_sysColor);
		return m_sysBrush;
	}
	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
