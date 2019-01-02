#include "Function.h"
#include "wfEEPROM.h"
#include "Variables.h"
#include "..\..\..\WF_Device\wfDefine.h"
#include "..\..\..\WF_Device\Verify.h"
#include "lcyHash.h"
#include "OnCarProc.h"

void GetDoorProc(uint8_t keyValue)
{
	uint8_t i,x,addr;
	uint8_t ram93[8];
// 	if(EE00==0)
// 		RomStateFlags.bStudy=1;
// 	if(!RomStateFlags.bStudy)
// 		keyValue=0x43;
	/************************************************************************/
	/* ����Ҫ�͵�Կ������                                                   */
	/************************************************************************/
	//1.��ü���ֵ
//  	addr=LeftTimes69&0x03;
//  	addr=addr+0x90;
//  	ButtonTimes=RomData_ReadByte(addr);
// 	//2,����ssid
 	//3,��ǰhash��һ��
   	for(i=0;i<8;i++)
   		ram93[i]=EISHash[i];

	//4,���������
	for(i=0;i<8;i++)
		DoorDatas[i]=ram93[i]^PSW[i];
	//5
	if(!RomStateFlags.bStudy)
	{	
		for(i=0;i<8;i++)
			DoorDatas[i]=PSW[i];
	}
	//6
	DoorDatas[0]^=ram93[0];
	DoorDatas[6]^=ButtonTimes;
	DoorDatas[7]^=keyValue;
	for(i=0;i<8;i++)
		lcyHashIn[i]=DoorDatas[i];
	lcyHashOnce();
	//7
//	x=ButtonTimes+1;
//	RomData_WriteByte(addr,x);
	//8
	if(RomStateFlags.bStudy)
	{
		for(i=0;i<8;i++)
			DoorDatas[i]=ram93[i]^lcyHashOut[i];
	}
	else
	{
		for(i=0;i<8;i++)
			DoorDatas[i]=lcyHashOut[i];
	}
}
//���ɻ�Ӧ���ض˵����ݣ���Ӧָ��Ϊ����0x26��������0x50
//responseCommanderΪ��Ӧ���0x26��0x50
void GetKeyWorkValue(uint8_t* rxList,uint8_t responseCommander)
{
	uint8_t i;
	for(i=0;i<8;i++)//��������յ�������������
	{
		WorkValueDatas[i]=PSW[i]^rxList[i];
	}
	//��4�ֽ���2425ָ����������
	WorkValueDatas[4]^=LeftTimes69;
	WorkValueDatas[5]^=LeftTimes[LeftTimesM];
	WorkValueDatas[6]^=LeftTimes[LeftTimesH];
	if(responseCommander==0x26)
	{
		if(RomStateFlags.bStudy)
			WorkValueDatas[7]^=0x24;
		else
			WorkValueDatas[7]^=0x25;
	}	
	else//0x50
	{
		WorkValueDatas[7]^=0xca;
	}
	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=WorkValueDatas[i];
	}
	lcyHashOnce();		
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	for(i=0;i<8;i++)
	{
		WorkValueDatas[i]=lcyHashOut[i]^curHash[i];
	}	
}
void GetKeyParam(void)
{
	uint8_t i, x,addr,t1;	
	if(!RomStateFlags.bRomWrited)
		return;
	//д���ˣ���ȡϵͳ����
	EE00 = RomData_ReadByte(0x00);
	EE00 >>= 6;
	RomData_ReadBytes(0x01, PSW, 8);
	RomData_ReadBytes(0x09, SSID, 4);
	//ʹ�ô�����λ
	//���69~70�洢���е�һ����Ϊ0xff�ĵ�Ԫ��������Ϊ�������
	//70-6f-6e-6d-6c-6b-6a-69-70
	GetCalcTimes69();
	//ʹ�ô�����λ
	GetLeftTimeBlock(LeftTimesH);
	//ʹ�ô�����λ
	GetLeftTimeBlock(LeftTimesM);
	//ʹ�ô�����λ���öΣ�ͬʱ�ҵ���ǰhash
	GetLeftTimeBlock(LeftTimesL);	
	x = LeftTimes69 & 0x03;
	if (x == 0)
	{
		RomData_ReadBytes(LeftTimesAddr[LeftTimesL], RomDatas, 8);
		for (i = 0; i < 8; i++)
		{
			curHash[i] = RomDatas[7 - i];
		}
	}
	else
	{
		addr=0xd0+x*10;
		CheckDataBlockVerify(addr);
		t1 = RomDatas[8];
		if(!gFlags.bFuncRet || RomDatas[9]==0 || t1!=LeftTimes69)//��ǰ�洢����û�е�ǰ��ϣֵ��������
		{
			RomData_ReadBytes(LeftTimesAddr[LeftTimesL], RomDatas, 8);
			for (i = 0; i < 8; i++)
			{
				lcyHashIn[i] = RomDatas[7 - i];
			}
			lcyHashCalc(x);
			for (i = 0; i < 8; i++)
			{
				RomDatas[7 - i] = lcyHashOut[i];
			}
			RomDatas[8] = LeftTimes69;
			RomDatas[9] = GetVerify_byteXOR(RomDatas,9);	
			RomData_WriteBytes(addr, RomDatas, 10);//д�뵱ǰ�����Σ���ʹ��ƥ��
			for (i = 0; i < 8; i++)
			{
				curHash[i] = RomDatas[7 - i];
			}
		}
		else//��ȷ��ȡ��hash
		{
			for (i = 0; i < 8; i++)
			{
				curHash[i] = RomDatas[7 - i];
			}
		}						
	}
	for(i=0;i<8;i++)
		lcyHashIn[i]=curHash[i];
	lcyHashOnce();
	for(i=0;i<8;i++)
		EISHash[i]=lcyHashOut[i];
	addr=LeftTimes69&0x03;
	addr=addr+0x90;
	ButtonTimes=RomData_ReadByte(addr);
}

//���ʹ�ô���
void GetLeftTimeBlock(uint8_t nBlock)
{
	uint8_t t1, t2, addr1, addr2, tTop;
	switch (nBlock)
	{
	case LeftTimesH:
		addr1 = 0x2d;
		addr2 = 0x37;
		tTop = 0x2f;
		break;
	case LeftTimesM:
		addr1 = 0x41;
		addr2 = 0x4b;
		tTop = 0x1f;
		break;
	case LeftTimesL:
		addr1 = 0x55;
		addr2 = 0x5f;
		tTop = 0x00;
		break;
	default:
		return;
	}
	CheckDataBlockVerify(addr1);
	t1 = RomDatas[8];
	if(!gFlags.bFuncRet)
	{
		t1 = 0xff;
	}
	CheckDataBlockVerify(addr2);
	t2 = RomDatas[8];
	if(!gFlags.bFuncRet)
	{
		t2 = 0xff;
	}
	LeftTimesAddr[nBlock] = 0xff;//�쳣ֵ
	if ((t1 != 0xff) && (t2 != 0xff))
	{
		if (nBlock != LeftTimesL)
		{
			if (t1 > t2)//�ô��
			{
				if ((t2 == 0) && (t1 == tTop))//���磺00 1f��ʹ��00
				{
					LeftTimesAddr[nBlock] = addr2;
					OtherLeftTimesAddr[nBlock] = addr1;
					LeftTimes[nBlock] = t2;
				}
				else//��������ʹ�ô��
				{
					LeftTimesAddr[nBlock] = addr1;
					OtherLeftTimesAddr[nBlock] = addr2;
					LeftTimes[nBlock] = t1;
				}
			}
			else//t2>t1
			{
				if ((t1 == 0) && (t2 == tTop))//���磺00 1f��ʹ��00
				{
					LeftTimesAddr[nBlock] = addr1;
					OtherLeftTimesAddr[nBlock] = addr2;
					LeftTimes[nBlock] = 0;
				}
				else//��������ʹ�ô��
				{
					LeftTimesAddr[nBlock] = addr2;
					OtherLeftTimesAddr[nBlock] = addr1;
					LeftTimes[nBlock] = t2;
				}
			}
			return;//����
		}
		else
		{
			tTop = LeftTimes69 & 0xfc;
			if (t1 == tTop)
			{
				LeftTimesAddr[nBlock] = addr1;
				OtherLeftTimesAddr[nBlock] = addr2;
				LeftTimes[nBlock] = t1;
				return;
			}
			else if (t2 == tTop)
			{
				LeftTimesAddr[nBlock] = addr2;
				OtherLeftTimesAddr[nBlock] = addr1;
				LeftTimes[nBlock] = t2;
				return;
			}
			else//���ζ���ƥ�䣬��Ҫ�������ɵ�λhash
			{
				t1 = 0xff;
				t2 = 0xff;
			}
		}
	}
	else if (t1 != 0xff)//��һ��ƥ�䣬���������ɵڶ���
	{
		LeftTimesAddr[nBlock] = addr1;
		OtherLeftTimesAddr[nBlock] = addr2;
		LeftTimes[nBlock] = t1;
	}
	else if (t2 != 0xff)
	{
		LeftTimesAddr[nBlock] = addr2;
		OtherLeftTimesAddr[nBlock] = addr1;
		LeftTimes[nBlock] = t2;
	}
	//IRTx_10_33_SSID();
	if (t1 != 0xff || t2 != 0xff)//��Ҫ�������ɵڶ���
	{

		switch (nBlock)
		{
		case LeftTimesH:
			if (LeftTimes[LeftTimesH] == 0)//��λΪ0�������ɵڶ���
				break;
			else
			{							
				t2 = LeftTimes[LeftTimesH] - 1;
				if (t2 >= 0x24)
				{
					t1 = 0x24;
					addr1 = 0x25;//RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152								
				}
				else if (t2 >= 0x18)
				{
					t1 = 0x18;
					addr1 = 0x1d;//RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
				}
				else if (t2 >= 0xc)
				{
					t1 = 0x0c;
					addr1 = 0x15;//RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
				}
				else
				{
					t1 = 0x00;
					addr1 = 0x0d;//RomData_ReadBytes(0x0d,RomDatas,8);//49152
				}
				// 							if ((LeftTimes[LeftTimesH] == 0x2e) ||
				// 								(LeftTimes[LeftTimesH] == 0x24) ||
				// 								(LeftTimes[LeftTimesH] == 0x18) ||
				// 								(LeftTimes[LeftTimesH] == 0x0c)
				// 								)//��ʱ���ɸ�λ��4096hash
				// 							{
				// 								RomData_ReadBytes(addr1, ref RomDatas, 8);
				// 								addr2 = 0xa0;
				// 								RomData_WriteBytes(addr2, RomDatas, 8);
				// 								for (i = 0; i < 8; i++)
				// 								{
				// 									lcyHashIn[i] = RomDatas[7 - i];
				// 								}
				// 								for (j = 1; j < 12; j++)
				// 								{
				// 									//HashCalc_N(4096);
				// 									// 				for (i = 0; i < 8; i++)
				// 									// 				{
				// 									// 					RomDatas[7 - i] = lcyHashOut[i];
				// 									// 				}
				// 									lcyHashIn = LCYHashCal.HashCal(lcyHashIn, 4096);
				// 									for (i = 0; i < 8; i++)
				// 									{
				// 										RomDatas[7 - i] = lcyHashIn[i];
				// 									}
				// 									addr2 += 8;
				// 									RomData_WriteBytes(addr2, RomDatas, 8);//д�뵱ǰ�����Σ���ʹ��ƥ��
				// 								}
				// 							}
				t1 = t2 - t1;
				// 							addr2=(uint8_t)(0xa0+(t1<<3));
				// 							RomData_ReadBytes(addr2, ref RomDatas, 8);
				// 							RomDatas[8] = (uint8_t)(LeftTimes[LeftTimesH] - 1);
				// 							//RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
				// 							RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
				// 							RomData_WriteBytes(OtherLeftTimesAddr[LeftTimesH], RomDatas, 10);//д�뵱ǰ�����Σ���ʹ��ƥ��
				FixDataBlock(OtherLeftTimesAddr[LeftTimesH], addr1, 4096, t1, LeftTimes[LeftTimesH] - 1);
			}
			break;
		case LeftTimesM:
			if (LeftTimes[LeftTimesM] == 0)//��λΪ0������Ҫ���λ��λ
			{
				if (LeftTimes[LeftTimesH] == 0)//��λΪ0�����޷���λ������������λ�ڶ���
					break;
				else//��λ������λ
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesH], 128, 0x1f, 0x1f);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesM], LeftTimesAddr[LeftTimesH], 128, LeftTimes[LeftTimesM] - 1, LeftTimes[LeftTimesM] - 1);
			}
			break;
		case LeftTimesL:
			if (LeftTimes[LeftTimesL] == 0)//��λΪ0������Ҫ����λ��λ
			{
				if ((LeftTimes[LeftTimesM] == 0) && (LeftTimes[LeftTimesH] == 0))//��λ����λ��Ϊ0�����޷���λ���������ɵ�λ�ڶ���
					break;
				else//��λ
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesM], 0x7c, 1, 0x7c);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesL], LeftTimesAddr[LeftTimesM], LeftTimes[LeftTimesL] - 4, 1, LeftTimes[LeftTimesL] - 4);
			}
			break;
		}
	}
	else//���ζ���ƥ�䣬��Ҫ��С�Ĵ�����������
	{
		t1 = RomData_ReadByte(addr1+8);
		t2 = RomData_ReadByte(addr2 + 8);
		if (t1 < t2)
		{
			LeftTimesAddr[nBlock] = addr1;
			OtherLeftTimesAddr[nBlock] = addr2;
			LeftTimes[nBlock] = t1;
		}
		else
		{
			LeftTimesAddr[nBlock] = addr2;
			OtherLeftTimesAddr[nBlock] = addr1;
			LeftTimes[nBlock] = t2;
		}
		switch (nBlock)
		{
		case LeftTimesH:
			if (LeftTimes[LeftTimesH] == 0)//��λΪ0������ɵ�һ��
			{
				RomData_ReadBytes(0x0d, RomDatas, 8);
				RomDatas[8] = 0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
				//RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
				RomData_WriteBytes(LeftTimesAddr[LeftTimesH], RomDatas, 10);//д�뵱ǰ�����Σ���ʹ��ƥ��
			}
			else
			{
				t2 = LeftTimes[LeftTimesH] - 1;
				if (t2 >= 0x24)
				{
					t1 = 0x24;
					addr1 = 0x25;//RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152
				}
				else if (t2 >= 0x18)
				{
					t1 = 0x18;
					addr1 = 0x1d;//RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
				}
				else if (t2 >= 0xc)
				{
					t1 = 0x0c;
					addr1 = 0x15;//RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
				}
				else
				{
					t1 = 0x00;
					addr1 = 0x0d;//RomData_ReadBytes(0x0d,RomDatas,8);//49152
				}
				t1 = t2 - t1;
				FixDataBlock(OtherLeftTimesAddr[LeftTimesH], addr1, 4096, t1, LeftTimes[LeftTimesH] - 1);//�޸�-1��
				FixDataBlock(LeftTimesAddr[LeftTimesH], OtherLeftTimesAddr[LeftTimesH], 4096, 1, LeftTimes[LeftTimesH]);//�޸���ǰ
			}
			break;
		case LeftTimesM:
			if (LeftTimes[LeftTimesM] == 0)//��λΪ0������Ҫ���λ��λ
			{
				RomData_ReadBytes(LeftTimesAddr[LeftTimesH], RomDatas, 8);
				RomDatas[8] = 0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
				//RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
				RomData_WriteBytes(LeftTimesAddr[LeftTimesM], RomDatas, 10);
				if (LeftTimes[LeftTimesH] == 0)//��λΪ0��������ɵ�ǰ��			
					break;
				else//��λ������λ
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesH], 128, 0x1f, 0x1f);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesM], LeftTimesAddr[LeftTimesH], 128, LeftTimes[LeftTimesM] - 1, LeftTimes[LeftTimesM] - 1);
				FixDataBlock(LeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesM], 128, 1, LeftTimes[LeftTimesM]);
			}
			break;
		case LeftTimesL:
			LeftTimes[LeftTimesL] = LeftTimes69 & 0xfc;
			if (LeftTimes[LeftTimesL] == 0)//��λΪ0������Ҫ����λ��λ
			{
				RomData_ReadBytes(LeftTimes[LeftTimesM], RomDatas, 8);
				RomDatas[8] = 0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
				//RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
				RomData_WriteBytes(LeftTimesAddr[LeftTimesL], RomDatas, 10);
				if ((LeftTimes[LeftTimesM] == 0) && (LeftTimes[LeftTimesH] == 0))//��λ����λ��Ϊ0�����޷���λ���������ɵ�λ�ڶ���
					break;
				else//��λ
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesM], 0x7c, 1, 0x7c);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesL], LeftTimesAddr[LeftTimesM], LeftTimes[LeftTimesL] - 4, 1, LeftTimes[LeftTimesL] - 4);
				FixDataBlock(LeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesL], 4, 1, LeftTimes[LeftTimesL]);
			}
			break;
		}
	}
}
//fixAddr:Ҫ�޸��ĵ�ַ
//hashAddr��ʹ�õ�hash�ĵ�ַxiuf
//stepLen:�޸���������
//times��Ŀ�����
void FixDataBlock(uint8_t fixAddr,uint8_t hashAddr,uint16_t stepLen,uint8_t loopTimes,uint8_t leftTiems)
{
	uint8_t i;
	RomData_ReadBytes(hashAddr, RomDatas, 8);
	if ((loopTimes != 0) && (stepLen!=0))
	{
		for (i = 0; i < 8; i++)
		{
			lcyHashIn[i] = RomDatas[7 - i];
		}
		for (i = 0; i < loopTimes; i++)
		{
		 	HashCalc_N(stepLen);
		}
		for (i = 0; i < 8; i++)
		{
		 	RomDatas[7 - i] = lcyHashOut[i];
		}
// 		for (i = 0; i < loopTimes; i++)
// 		{
// 			lcyHashIn=LCYHashCal.HashCal(lcyHashIn, stepLen);					
// 		}
// 		for (i = 0; i < 8; i++)
// 		{
// 			RomDatas[7 - i] = lcyHashIn[i];
// 		}
	}
	RomDatas[8] = leftTiems;
	RomDatas[9] = GetVerify_byteXOR(RomDatas,9);	
	RomData_WriteBytes(fixAddr, RomDatas, 10);//д�뵱ǰ�����Σ���ʹ��ƥ��
}
//sub_1442_USEDEC:
//ʹ�ô�����һ
void UsedDEC(void)
{
	uint8_t x;
	if (LeftTimes69 == 0)//0-1��Ҫ��λ��д��0x7f
	{
		if (LeftTimes[LeftTimesM] == 0 && LeftTimes[LeftTimesH] == 0)//���д����Ѿ��þ�
		{
			NVIC_SystemReset();//��λ
		}
		else
		{
			ReverseRom(LeftTimesAddr[LeftTimesM] + 9);//�ƻ���λ��ǰʹ�ö�					
			if (LeftTimes[LeftTimesM] == 0)//�ж�ʹ�ô���Ϊ�㣬����Ҫ���λ��λ
			{
				ReverseRom(LeftTimesAddr[LeftTimesH] + 9);
			}
			ReverseRom(LeftTimesAddr[LeftTimesL] + 9);//�ƻ���λ��ǰʹ�ö�

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
// 			ReverseRom(OtherLeftTimesAddr[LeftTimesM] + 9);//�ƻ���λ��ǰʹ�ö�					
// 			if (LeftTimes[LeftTimesM] == 0)//�ж�ʹ�ô���Ϊ�㣬����Ҫ���λ��λ
// 			{
// 				ReverseRom(OtherLeftTimesAddr[LeftTimesH] + 9);
// 			}
// 			ReverseRom(OtherLeftTimesAddr[LeftTimesL] + 9);//�ƻ���λ��ǰʹ�ö�
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			LeftTimes69 = 0x7f;
		}
	}
	else
	{
		x = LeftTimes69 & 0x03;
		if (x == 0x00)//��Ҫ�л���λ��
		{
			ReverseRom(LeftTimesAddr[LeftTimesL] + 9);//�ƻ���λ��ǰʹ�ö�
		}
		LeftTimes69--;//ʹ�ô�����һ
	}
	RomData_WriteByte(LeftTimes69Addr, 0xff);//ԭ��ַд0xff��дһ��д���һ��
	if (LeftTimes69Addr == 0x69)
	{
		RomData_WriteByte(0x70, LeftTimes69);
	}
	else
	{
		RomData_WriteByte(LeftTimes69Addr - 1, LeftTimes69);
	}	
}
//��תָ����ַ�����ݣ���Ϊ0xff��дΪ0����֮������0xff��дΪ0xff
void ReverseRom(uint8_t addr)
{
	if (RomData_ReadByte(addr) != 0xff)
	{
		RomData_WriteByte(addr, 0xff);
	}
	else
	{
		RomData_WriteByte(addr, 0);
	}
}
//sub_13EB_Get69to70_FirstNFF
//���69~70�洢���е�һ����Ϊ0xff�ĵ�Ԫ��������Ϊ�������
//70-6f-6e-6d-6c-6b-6a-69-70
void GetCalcTimes69(void)
{
	uint8_t i, x;
	RomData_ReadBytes(0x69, RomDatas, 8);
	for (i = 8; i != 0; i--)
	{
		x = i - 1;
		if (RomDatas[x] != 0xff)
			break;
	}
	if (i == 0)//û�з�ff��Ԫ,������0x69д��00�����ٵĵ�λ����
	{
		LeftTimes69 = 0;
		RomData_WriteByte(0x69, 0);
		// 		CalcTimes_D2=0;
		// 		CalcTimes_BF=0;
		LeftTimes69Addr = 0x69;
		return;
	}
	else
	{
		//�ݴ����ж���һ���Ƿ�ҲΪ��ff
		if (i == 1)//�������ж�
		{
			LeftTimes69 = RomDatas[0];
			// 			CalcTimes_D2=CalcTimes[0]&0xfc;
			// 			CalcTimes_BF=CalcTimes[0]&0x03;
			LeftTimes69Addr = 0x69;
			return;
		}
		else
		{
			x = i - 2;
			if (RomDatas[x] != 0xff)//������������ʹ����һ��
			{
				LeftTimes69 = RomDatas[x];
				// 				CalcTimes_D2=CalcTimes[0]&0xfc;
				// 				CalcTimes_BF=CalcTimes[0]&0x03;
				LeftTimes69Addr = 0x69 + x;
				ReverseRom(0x68 + i);//����rom
			}
			else//û����ʹ�ô�ֵ
			{
				x = i - 1;
				LeftTimes69 = RomDatas[x];
				// 				CalcTimes_D2=CalcTimes[0]&0xfc;
				// 				CalcTimes_BF=CalcTimes[0]&0x03;
				LeftTimes69Addr =0x69 + x;
			}
		}
		//rom�ݴ�:���ж�ǰһ���Ƿ�ҲΪ��ff,ֻ��70��Ԫʱ��Ҫ�ж�
		if (i == 8)//70��Ԫ
		{
			if (RomDatas[0] != 0xff)
			{
				ReverseRom(0x69);
			}
		}
	}
}
void HashCalc_N(uint32_t nCount)
{
	uint32_t i;
	for(i=0;i<nCount;i+=4)
	{
		lcyHashCalc(4);
		if(bOnCarPower())
			CarIRTx_10_33_SSID();
	}
}
//������ݶ�У��
void CheckDataBlockVerify(uint8_t Addr)
{
	uint8_t x;
	gFlags.bFuncRet=0;	
	RomData_ReadBytes(Addr,RomDatas,10);
	if(RomDatas[8]>0x7f)
	{
		gFlags.bFuncRet=0;
		return;
	}
	x=GetVerify_byteXOR(RomDatas,9);
	if(x==RomDatas[9])
		gFlags.bFuncRet=1;
}
void GetKeyState(void)
{
	uint8_t i, x;
	RomData_ReadBytes(0x9e,RomDatas, 2);
	x = RomDatas[0];
	x += RomDatas[1];
	if((RomDatas[0]==0x00) || (x!=0x00))//�ڴ�״̬У��ʧ��
	{
		ChangeKeyState(ROM_9E);
	}	
	RomData_ReadBytes(0x9e, RomDatas, 2);
	x = RomDatas[0];
	x += RomDatas[1];
	RomStateFlags.bRomWrited = 0;
	RomStateFlags.bStudy = 0;
	if (x != 0)
		NVIC_SystemReset();//�洢���޷���ʼ����ϵͳ��λ
	EE9e=RomDatas[0]&0x7f;
	x=EE9e&0xee;
	if (x==0x04)//д����
	{
		RomStateFlags.bRomWrited = 1;
	}
	if (GetBit(RomDatas[0], 4) != 0)//ѧϰ��
	{
		RomStateFlags.bStudy = 1;
	}
}
void ChangeKeyState(uint8_t state)
{
	uint8_t x[2];
	x[0]=state;
	x[1]=0x00-state;
	RomData_WriteBytes(0x9e,x,2);
}
void VerifyEEDatas(uint8_t maxNum,uint8_t lastAddr)
{
	uint8_t i,j,addr;
	uint8_t ee[8];
	gFlags.bFuncRet=0;
	switch(CarIRCommand)
	{
	case 0x0e:
		addr=0x88;
		break;
	case 0x00:
		addr=0x80;
		break;
	case 0x03:
		addr=0xfe;
		break;
	default:
		addr=0x00;
		break;
	}
	for(i=0;i<8;i++)
		ee[i]=0x00;
	for(i=0;i<maxNum;i++)
	{
		RomData_ReadBytes(addr,RomDatas,8);
		if(addr==0xfe)
		{
			RomDatas[0]=0;
			RomDatas[1]=0;
		}
		for(j=0;j<8;j++)
		{
			lcyHashIn[i]=RomDatas[i]^ee[i];
		}
		lcyHashOnce();
		for(j=0;j<8;j++)
		{
			ee[i]=lcyHashOut[i];
		}
		addr+=8;
	}
	RomData_ReadBytes(lastAddr,RomDatas,8);
	for(i=0;i<8;i++)
	{
		if(ee[i]!=RomDatas[i])
			return;
	}
	gFlags.bFuncRet=1;
}
