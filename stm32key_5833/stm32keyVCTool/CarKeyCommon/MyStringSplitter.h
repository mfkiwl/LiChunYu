#pragma once
/*
ʹ�÷�����
CSplitStr Split;
Split.SetSplitFlag(",");//�趨�ָ�����Ĭ��ֵҲ�ǡ�����
Split.SetSequenceAsOne(TRUE);//�趨��ϵ�ķָ����δ���
Split.SetData(m_sText);//��ȡҪ�ָ���ַ���
CStringArray array;//��ȡ�����ŵ�����
Split.GetSplitStrArray(array);
for (int i=0; i< array.GetSize(); i++)
{
m_List.AddString(array.GetAt(i));

}

*/
class CMySplitStr  
{
private:
	//�������зֵı�־���ŵ���һ����־����
	BOOL    m_bSequenceAsOne;
	//���зֵ��ı�
	CString m_sData;
	//�зֵı�־����
	CString m_sSplitFlag;
public:
	//�õ��зֺõ��ı���
	void GetSplitStrArray(CStringArray &array);
	//�õ����зֵ��ı�
	CString GetData();
	//���ñ��зֵ��ı�
	void SetData(CString sData,bool bTrim=false);
	//�õ��зֲ���
	BOOL GetSequenceAsOne() {return m_bSequenceAsOne;};
	//�����зֲ���,�����ķָ����Ƿ�һ������trueΪ��һ������
	void SetSequenceAsOne(BOOL bSequenceAsOne) {m_bSequenceAsOne = bSequenceAsOne;};
	//�õ��зֱ�־
	CString GetSplitFlag() {return m_sSplitFlag;};
	//�����зֱ�־
	void SetSplitFlag(CString sSplitFlag) {m_sSplitFlag = sSplitFlag;};

	CMySplitStr();
	virtual ~CMySplitStr();

};
