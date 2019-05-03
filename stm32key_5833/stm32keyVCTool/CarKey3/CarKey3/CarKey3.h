
// CarKey3.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#ifdef _UseCopy
#include ".\..\..\CarKeyCommon\HIDDevice.h"
#else
#include "..\..\CarKeyCommon\HIDWork.h"
#endif
#include "..\..\CarKeyCommon\WithServer.h"

// CCarKey3App:
// �йش����ʵ�֣������ CarKey3.cpp
//

class CCarKey3App : public CWinApp
{
public:
	CCarKey3App();
	static void WaitMS(int ms);
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	static CString RunPath;
	static CWithServer withServer;
	static float m_fSoftVer;
	static float m_fHexVer;
#ifdef _UseCopy
	static CHIDDevice TheHIDDevice;
#else
	static CHIDWork TheHIDDevice;
	static bool m_bPass;
	static byte PassSSID[4];
	static byte HPassword[8];
	static CString strLogFormat;
	static WORD LANGID;
#endif
	DECLARE_MESSAGE_MAP()
};

extern CCarKey3App theApp;