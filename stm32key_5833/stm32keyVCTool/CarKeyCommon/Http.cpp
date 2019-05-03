#include "StdAfx.h"
#include "Http.h"
#include "CommFunc.h"

CHttp::CHttp(void)
{
	m_cookies = "";
	m_CodePage = -1;
	//ͼƬ���ڴ�����ֵ
	m_hGlobal = NULL;
	m_pStream = NULL;
}

CHttp::~CHttp(void)
{
	//�ͷ�ͼƬ�ڴ����
	if (m_hGlobal != NULL)
		GlobalFree(m_hGlobal);
	if (m_pStream != NULL)
		m_pStream->Release();
}

///////////////////////////////////////////////////////////////////
/*
���ܣ��ύGET����
������URL��ַ
���أ���õ�HTMLҳ������
*/
CString CHttp::get(CString url)
{
	return GetHTML(url, _T(""), _T(""));
}

///////////////////////////////////////////////////////////////////
/*
���ܣ��ύGET����
������URL��ַ������ͷ���ַ���
���أ���õ�HTMLҳ������
*/
CString CHttp::get(CString url, CString header)
{
	return GetHTML(url, header, _T(""));
}

///////////////////////////////////////////////////////////////////
/*
���ܣ��ύPOST����
������URL��ַ��POST���ݵ��ַ���
���أ���õ�HTMLҳ������
*/
CString CHttp::post(CString url, CString PostData)
{
	return GetHTML(url, _T(""), PostData, true);
}

///////////////////////////////////////////////////////////////////
/*
���ܣ��ύPOST����
������URL��ַ������ͷ�ַ�����POST���ݵ��ַ���
���أ���õ�HTMLҳ������
*/
CString CHttp::post(CString url, CString header, CString PostData)
{
	return GetHTML(url, header, PostData, true);
}

////////////////////////////////////////////////////////////////////////
/*
���ܣ��ύHTML����
������
	URL��ַ��
	����ͷ��
	POST���ݣ�
	�Ƿ�ʹ��POST��ʽ�ύ��Ĭ��Ϊ����
���أ���õ�HTMLҳ������
*/
CString CHttp::GetHTML(CString url, CString header, CString PostData, bool isPost)
{
	//��ȡURL���������ļ�·��
	CString domain;
	CString path;
	DecomposeURL(url, domain, path);
	
	CHttpConnection *pConnection;
	if(domain.Find(_T(":"))!=-1)
	{
		CString s1,s2;
		s1=domain.Mid(0,domain.Find(_T(":")));
		s2=domain.Mid(domain.Find(_T(":"))+1);
		int port=  _ttoi(s2) ;
		pConnection = m_session.GetHttpConnection(s1,(INTERNET_PORT)port,NULL,NULL);
	}
	else
	{
		pConnection = m_session.GetHttpConnection(domain);
	}
	//��������	
	CHttpFile *pFile = pConnection->OpenRequest
	(
		(isPost) ? CHttpConnection::HTTP_VERB_POST : CHttpConnection::HTTP_VERB_GET,
		path, 0, 1, 0, 0, INTERNET_FLAG_NO_COOKIES
	);
	//�������ͷ
	if (header != "")
		pFile->AddRequestHeaders(CString(header + _T("\n")));

	//���cookies
	if (m_cookies != "")
		pFile->AddRequestHeaders(CString(_T("Cookie: ") + m_cookies));

	//��POST���ݱ���
	int len;
	char* pp;
 	if (isPost)
// 		PostData = EncodePOST(PostData);
	{
		//=CCommFunc::GetCStringLength(PostData);
		pp=CCommFunc::CStringToChar(PostData,&len);
	}
	try
	{
		//�ύ����
		if (isPost)
		{
			pFile->SendRequest(CString("Content-Type:application/x-www-form-urlencoded"), pp, len);
			delete[] pp;
		}
			//pFile->SendRequest(CString("Content-Type:application/x-www-form-urlencoded"), PostData.GetBuffer(), PostData.GetLength());
		else
			pFile->SendRequest();

		//��ⷵ����
		DWORD StatusCode;
		pFile->QueryInfoStatusCode(StatusCode);
		//��������벻��200�������쳣����
		if (StatusCode != HTTP_STATUS_OK)
		{
			pFile->Close();
			pConnection->Close();
			delete pFile;
			delete pConnection;
			//�쳣���ؿմ�
			return CString("");
		}
	}
	catch (CException* e)// (...)
	{
		//�ͷ�����
		pFile->Close();
		pConnection->Close();
		delete pFile;
		delete pConnection;
		//�쳣���ؿմ�
		return CString("");
	}

	//��ȡ����HTML������
	CString RevBuf;
	RevBuf = RevHTML(pFile);

	//��ȡ�����淵�ص�cookies
	SaveCookies(pFile);

	//�ͷ�����
	pFile->Close();
	pConnection->Close();
	delete pFile;
	delete pConnection;
	return RevBuf;
}

////////////////////////////////////////////////////////////////////
/*
���ܣ���ȡURL�е��������ļ�·��
������URL��ַ�������������ַ����������ļ�·�����ַ���
���أ���
*/
void CHttp::DecomposeURL(CString &url, CString &domain, CString &path)
{
	int pos1, pos2;
	pos1 = url.Find('/');
	pos1 += 2;
	pos2 = url.Find('/', pos1);
	if(pos2==-1)
	{
		domain=url.Mid(pos1,url.GetLength()-pos1);
		path=_T("");
	}
	else
	{
		pos2--;
		domain = url.Mid(pos1, pos2 - pos1 + 1);

		pos2 += 2;
		path = url.Right(url.GetLength() - pos2);
	}
}

/////////////////////////////////////////////////////////////////////
/*
���ܣ����շ��ص�HTML��Ϣ
������CHttpFile�ļ�ָ��
���أ�HTML���ݵ��ַ���
*/
CString CHttp::RevHTML(CHttpFile *pFile)
{
//  	CString strHtml;
//  	TCHAR buf[1024]= {0};  
//  	char tmp[2048] = {0};  
//  	while(pFile->ReadString(buf, 1023))   
//  	{  
//  		for(int i=0, j=0; i<lstrlen(buf); i++, j+=2)  
//  		{  
//  			tmp[j] = LOBYTE(buf[i]);//ȡ�ֵĵ��ֽ�  
//  			tmp[j+1] = HIBYTE(buf[i]);//ȡ�ֵĸ��ֽ�  
//  		}  
//  		CString ss(tmp);  
//  		strHtml+=ss;  
//  	}
//  	return strHtml;
 	CString RevBuf;
 	
 	//��ȡ���ص��ļ�����
 	while (true)
 	{
 		char buf[1024];
 		int len;
 		char tmp[2048] = {0}; 
 		try
 		{
 			len= pFile->Read(buf, 1023);
 		}
 		catch (...)
 		{
 			//�쳣���ؿմ�
 			return CString("");
 		}
 
 		if (len <= 0)
 			break;
 		buf[len] = '\0';
 		RevBuf += buf;
 	}
 
 	//�Է��ص�������ȷ����
 	RevBuf = EncodeHTML(RevBuf);
 
 	return RevBuf;
}

////////////////////////////////////////////////////////////////////
/*
���ܣ�����ת��
������Դ�ַ���
���أ�ת������ַ���
*/
CString CHttp::EncodeHTML(CString &str)
{
	//�����ǰδ���ñ���
	if (m_CodePage == -1)
	{
		//���ݱ�ǩ�����ñ���
		int pos1 = str.Find(_T("charset"));
		if (pos1 == -1)
			pos1 = str.Find(_T("encoding"));
		if (pos1 > -1)
		{
			int pos2 = str.Find('>', pos1);
			CString CodePageStr;
			CodePageStr = str.Mid(pos1, pos2 - pos1);
			CodePageStr.MakeUpper();
			if (CodePageStr.Find(_T("UTF-8")) > -1)
				m_CodePage = CP_UTF8;
			else
				if (CodePageStr.Find(_T("GB2312")) > -1)
					m_CodePage = CP_ACP;
				else
					if (CodePageStr.Find(_T("BGK")) > -1)
						m_CodePage = CP_ACP;
		}
	}

// 	//����������������룬��ת������
// 	if (m_CodePage == CP_UTF8)
// 	{
// 		//���ַ���ת��ΪUNICODE����
// 		CStringW strW;
// 		//��ȡUNICODE��������ĳ���
// 		int len = ::MultiByteToWideChar(m_CodePage, NULL, str.GetBuffer(), str.GetLength(), NULL, NULL);
// 		//ע������ˣ������ַ���������
// 		strW.GetBuffer(len + 1);
// 		::MultiByteToWideChar(m_CodePage, NULL, str.GetBuffer(), str.GetLength() + 1, strW.GetBuffer(), len + 1);
// 		strW.ReleaseBuffer();
// 
// 		//��UNICODE����ת��ΪGB2312����
// 		CString strA;
// 		BOOL succ;
// 		//��ȡGB2312��������ĳ���
// 		len = ::WideCharToMultiByte(CP_ACP, NULL, strW.GetBuffer(), strW.GetLength(), NULL, NULL, NULL, &succ);
// 		//ע������ˣ������ַ���������
// 		strA.GetBuffer(len + 1);
// 		::WideCharToMultiByte(CP_ACP, NULL, strW.GetBuffer(), strW.GetLength() + 1, strA.GetBuffer(), len + 1, NULL, &succ);
// 		strA.ReleaseBuffer();
// 	
// 		return strA;
// 	}

	//����ת�����򷵻�ԭ��
	return str;
}

////////////////////////////////////////////////////////////////////
/*
���ܣ�POST���ݱ���ת��
������Դ�ַ���
���أ�ת������ַ���
*/
CString CHttp::EncodePOST(CString &str)
{
	//����������������룬��ת������
// 	if (m_CodePage == CP_UTF8)
// 	{
// 		//��GB2312����ת��ΪUNICODE����
// 		CStringW strW;
// 		//��ȡUNICODE��������ĳ���
// 		int len = ::MultiByteToWideChar(CP_ACP, NULL, str.GetBuffer(), str.GetLength(), NULL, NULL);
// 		//ע������ˣ������ַ���������
// 		strW.GetBuffer(len + 1);
// 		::MultiByteToWideChar(CP_ACP, NULL, str.GetBuffer(), str.GetLength() + 1, strW.GetBuffer(), len + 1);
// 		strW.ReleaseBuffer();
// 
// 		//��UNICODE����ת��ΪĿ�����
// 		CString strA;
// 		//��ȡGB2312��������ĳ���
// 		len = ::WideCharToMultiByte(m_CodePage, NULL, strW.GetBuffer(), strW.GetLength(), NULL, NULL, NULL, NULL);
// 		//ע������ˣ������ַ���������
// 		strA.GetBuffer(len + 1);
// 		::WideCharToMultiByte(m_CodePage, NULL, strW.GetBuffer(), strW.GetLength() + 1, strA.GetBuffer(), len + 1, NULL, NULL);
// 		strA.ReleaseBuffer();
// 	
// 		return strA;
// 	}

	//����ת�����򷵻�ԭ��
	return str;
}

//////////////////////////////////////////////////////////////////////
/*
���ܣ���ȡ�����淵�ص�cookies
������CHttpFile�ļ�ָ��
���أ���
*/
void CHttp::SaveCookies(CHttpFile *pFile)
{
	//�ж��cookies��ʱ����Ҫindex����
	DWORD index = 0;

	while (true)  
	{
		//��ȡһ��cookie
		CString cookie;
		char CookieBuf[1024];
		DWORD BufLen = 1023;
		//QueryInfo������CString�������غ�����BUG������ʹ��������غ���
		if (pFile->QueryInfo(HTTP_QUERY_SET_COOKIE, CookieBuf, &BufLen, &index) == false) 
			break;
		cookie = CookieBuf;
		//�Ӹ��ֺŷ�����Һ���ȡ
		cookie += ';';

		//��ȡcookie�������
		int ItemCount = 0;
		//���������
		for (int i = 0; i < cookie.GetLength(); i++)
			if (cookie[i] == ';')
				ItemCount++;
		int pos1 = 0;
		int pos2 = 0;
		//���ÿһ��
		for (int i = 0; i < ItemCount; i++)
		{
			//��ȡһ��
			pos1 = pos2;
			if (cookie[pos1] == ';')
				pos1++;
			pos2 = cookie.Find(';', pos1);
			CString CookieItem = cookie.Mid(pos1, pos2 - pos1);

			int pos = CookieItem.Find('=');
			//���账���cookie��Ŀ
			if (pos == -1)
				continue;
			//��ȡ����
			CString ItemName = CookieItem.Left(pos);
			ItemName.Trim();

			//����cookie���Ƿ���Ҫ����
			CString ItemNameCheck = ItemName;
			ItemNameCheck.MakeLower();
			if (ItemNameCheck == "expires" || ItemNameCheck == "domain" || ItemNameCheck == "path")
				continue;

			//����Ƿ��Ѿ����ڴ�cookie��
			int ItemPos = m_cookies.Find(ItemName);
			if (ItemPos > -1 && 
				(ItemPos == 0 || m_cookies[ItemPos - 1] == ' ' || m_cookies[ItemPos - 1] == ';') &&
				(m_cookies[ItemPos + ItemName.GetLength()] == ' ' || m_cookies[ItemPos + ItemName.GetLength()] == '='))
			{
				//���ڴ�cookie���Ҫ�滻
				int ItemPosEnd = m_cookies.Find(';', ItemPos);
				CString CutLeft = m_cookies.Left(ItemPos);
				CutLeft.Trim();
				CString CutRight = m_cookies.Right(m_cookies.GetLength() - ItemPosEnd);
				//��������cookies��
				m_cookies = CutLeft + CookieItem + CutRight;
			}
			else
				//��������ֱ�����
				m_cookies += CookieItem + ';';
		}
	}
}

/////////////////////////////////////////////////////////////////////
/*
���ܣ���ȡͼƬ
������ͼƬ��URL��ַ
���أ�
	CImage����ָ�룬��OnDraw����OnPaint�е���Draw����������ʾͼƬ��
	ʧ�ܷ��ؿ�ָ��
*/
CImage *CHttp::GetImg(CString url)
{
	//��ȡURL���������ļ�·��
	CString domain;
	CString path;
	DecomposeURL(url, domain, path);

	//��������
	CHttpConnection *pConnection = m_session.GetHttpConnection(domain);
	CHttpFile *pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, path,
		0, 1, 0, 0, INTERNET_FLAG_NO_COOKIES);

	try
	{
		//�ύ����
		pFile->SendRequest();

		//��ⷵ����
		DWORD StatusCode;
		pFile->QueryInfoStatusCode(StatusCode);
		//��������벻��200�������쳣����
		if (StatusCode != HTTP_STATUS_OK)
			throw false;
	}
	catch (...)
	{
		//�ͷ�����
		pFile->Close();
		pConnection->Close();
		delete pFile;
		delete pConnection;
		//�쳣���ؿ�ָ��
		return NULL;
	}

	//��ȡͼƬ����
	char *ImgData = NULL;
	int ImgSize = 0;
	while (true)
	{
		char Data[1024];
		int len;

		try
		{
			len = pFile->Read(Data, 1024);
		}
		catch (...)
		{
			//�ͷ�����
			pFile->Close();
			pConnection->Close();
			delete pFile;
			delete pConnection;
			//�쳣���ؿ�ָ��
			return NULL;
		}		
		
		if (len <= 0)
			break;
		//����ͼƬ����
		ImgSize += len;
		ImgData = (char *)realloc(ImgData, ImgSize);
		memcpy(ImgData + ImgSize - len, Data, len);
	}

	//�������ڴ�
	m_hGlobal = GlobalAlloc(GMEM_MOVEABLE, ImgSize);
	void *pData = GlobalLock(m_hGlobal);
	//��������
	memcpy(pData, ImgData, ImgSize);
	free(ImgData);
	GlobalUnlock(m_hGlobal);

	//������
	m_pStream = NULL;
	if (CreateStreamOnHGlobal(m_hGlobal, TRUE, &m_pStream) == S_OK)
	{
		//�ͷ��Ѿ����ص�ͼƬ����ֹ�ڶ��μ��ش���
		m_img.Destroy();
		//����ͼƬ
		if (!SUCCEEDED(m_img.Load(m_pStream)))
		{
			//�ͷ�����
			pFile->Close();
			pConnection->Close();
			delete pFile;
			delete pConnection;
			//ʧ�ܷ��ؿ�ָ��
			return NULL;
		}
	}

	//��ȡ�����淵�ص�cookies
	SaveCookies(pFile);

	//�ͷ�����
	pFile->Close();
	pConnection->Close();

	return &m_img;
}

