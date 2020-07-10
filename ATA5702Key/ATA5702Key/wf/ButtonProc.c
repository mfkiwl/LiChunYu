#include "ButtonProc.h"
#include "Variables.h"
#include "RF_TX.h"
#include "EEProc.h"
#include <util\delay.h>

uint8_t KeyData[8];
uint8_t keyValue;
uint8_t KeyTimes;
void GetKeyData(void);
void keyRFIRTx(void);
void keyRFTx(void);
void ReadButton(void)
{
	curKeyStateFlags=ReadButton_PD();
	if(curKeyStateFlags==NoKey)
	{
		return;
		//NVIC_SystemReset();
	}
	_delay_ms(20);
	if(curKeyStateFlags!=ReadButton_PD())
	{
		curKeyStateFlags=NoKey;
		return;
		//NVIC_SystemReset();
	}
}
void ButtionProc(void)
{
	uint8_t i,addr,x,y;	
	uint16_t li;
 	switch(curKeyStateFlags)
 	{
 	case  FindCarKey:
 		keyValue=0x2b;
 		break;
 	case  LockKey:
 		keyValue=0x23;
 		break;
 	case  UnLockKey:
 		keyValue=RomData_ReadByte(EEDataOriginAddr+0x94);
 		if(keyValue!=0)
 			keyValue=0x21;
 		else
 			keyValue=0x20;
 		break;
 	case  TailGateKey:
 		keyValue=0x22;
 		break;
 	default:
 		return;
 	}
	//BAT_ON();	
	ATA_rfTxInit_C();
	
	//GetKeyParam();//���Կ�׵�ǰ�������		
	if(curKeyStateFlags==TailGateKey || curKeyStateFlags==FindCarKey)//����û�ж̰�
	{
		if(curKeyStateFlags==FindCarKey)
		{
			keyValue=0x29;
		}
		else
		{
			keyValue=0x45;
		}
		if(RomStateFlags.Bits.bRomWrited)//�����ɷ�������
		{
			GetDoorProc(keyValue);
			/************************************************************************/
			/*  �޸İ�������                                                        */
			/************************************************************************/
			addr=LeftTimes69&0x03;
			addr=addr+0x90;
			x=RomData_ReadByte(EEDataOriginAddr+addr);
			x++;
			RomData_WriteByte(EEDataOriginAddr+addr,x);
		}
		keyRFTx();
		_delay_ms(150);
		keyRFTx();
		_delay_ms(112);
		oldKeyStateFlags=curKeyStateFlags;
		ReadButton();
		if(curKeyStateFlags!=oldKeyStateFlags)//�����仯��λ
		{
			//PowerLed();
			//BAT_OFF();
			return;
		}
		if(curKeyStateFlags==FindCarKey)
		{
			keyValue=0x2b;
		}
		else
		{
			keyValue=0x22;
		}
		if(RomStateFlags.Bits.bRomWrited)//�����ɷ�������
		{
			addr=LeftTimes69&0x03;
			addr=addr+0x90;
			ButtonTimes=RomData_ReadByte(EEDataOriginAddr+addr);
			GetDoorProc(keyValue);
			/************************************************************************/
			/*  �޸İ�������                                                        */
			/************************************************************************/
			addr=LeftTimes69&0x03;
			addr=addr+0x90;
			x=RomData_ReadByte(EEDataOriginAddr+addr);
			x++;
			RomData_WriteByte(EEDataOriginAddr+addr,x);
		}
		keyRFTx();
		_delay_ms(95);
		keyRFIRTx();
		_delay_ms(95);	
		keyRFTx();
		PowerLed();
		//BAT_OFF();
// 		while(1)
// 		{
// 			
// 		}
		return;
	}
	else
	{		
		if(RomStateFlags.Bits.bRomWrited)//�����ɷ�������
		{
			GetDoorProc(keyValue);
			/************************************************************************/
			/*  �޸İ�������                                                        */
			/************************************************************************/
			addr=LeftTimes69&0x03;
			addr=addr+0x90;
			x=RomData_ReadByte(EEDataOriginAddr+addr);
			x++;
			RomData_WriteByte(EEDataOriginAddr+addr,x);
		}
		keyRFTx();
		_delay_ms(95);
		keyRFIRTx();
		if(keyValue!=0x2b)//Ѱ��
		{
			//ad����ѹ
		}
		PowerLed();
		oldKeyStateFlags=curKeyStateFlags;
		ReadButton();	
		if(curKeyStateFlags==NoKey)//�����仯��λ
		{
			_delay_ms(95);	
			keyRFTx();
			//BAT_OFF();
			return;
		}
		else if(curKeyStateFlags!=oldKeyStateFlags)//�����仯��λ
		{
			//BAT_OFF();
			return;
		}
		//BAT_OFF();
		/************************************************************************/
		/* �ٴ���������                                                       */
		/************************************************************************/
		if((curKeyStateFlags==UnLockKey) || (curKeyStateFlags==LockKey))
		{
			//_delay_ms(16);
			//LED_ON();
			for(li=0;li<0x0270;i++)
			{
				IRTxList[0]=0x30;
				IRTxCount=1;
				RFIRTxProc();
				_delay_ms(3);
				oldKeyStateFlags=curKeyStateFlags;
				ReadButton();
				if(curKeyStateFlags!=oldKeyStateFlags)//�����仯��λ
				{
					if(curKeyStateFlags==NoKey)
						ReadButton();
					if(curKeyStateFlags==Lock_UnLock_Key)//�л�������ֵ
					{
						for(li=0;li<250;li++)
						{
							ReadButton();
							if(curKeyStateFlags!=Lock_UnLock_Key)
							{
								LED_OFF();
								return;
							}
						}
						//BAT_ON();
						ReverseRom(0x94);
						//BAT_OFF();
						LED_ON();
						while(1)
						{
							curKeyStateFlags=ReadButton_PD();
							if(curKeyStateFlags==NoKey)
							{
								_delay_ms(20);
								curKeyStateFlags=ReadButton_PD();
								if(curKeyStateFlags==NoKey)
								{
									break;
								}
							}
						}
						for(i=0;i<10;i++)
						{
							LED_ON();
							_delay_ms(200);
							LED_OFF();
							_delay_ms(200);
						}
						return;
					}
					else if(curKeyStateFlags==UnLock_TailGate_Key)//Կ�ײ���
					{
						for(li=0;li<500;li++)//10s
						{
							ReadButton();
							if(curKeyStateFlags!=UnLock_TailGate_Key)
							{
								LED_OFF();
								return;
							}
						}
						LED_ON();
						//�ȴ��ɿ���ǰ��ϼ�
						while(1)
						{
							//ReadButton();
							curKeyStateFlags=ReadButton_PD();
							if(curKeyStateFlags==NoKey)
							{
								_delay_ms(20);
								curKeyStateFlags=ReadButton_PD();
								if(curKeyStateFlags==NoKey)
								{
									break;
								}
							}
						}
						x=0;
						for(li=0;li<250;li++)//5s
						{
							ReadButton();
							if(x&0x01)//�������ɿ�
							{
								if(curKeyStateFlags==NoKey)
								{
									x++;
									li=0;
									if(x==6)
									{
										//�ɹ�����
										ChangeKeyState(ROM_9E);
										for(i=0;i<10;i++)
										{
											LED_ON();
											_delay_ms(200);
											LED_OFF();
											_delay_ms(200);
										}
										break;
									}
								}
								else if(curKeyStateFlags!=UnLockKey)
								{
									LED_OFF();
									return;
								}
							}
							else//˫���Ȱ���
							{
								if(curKeyStateFlags==UnLockKey)
								{
									x++;
								}
								else if(curKeyStateFlags==NoKey)
								{
									_delay_ms(20);
								}
								else
								{
									LED_OFF();
									return;
								}
							}
						}
						LED_OFF();
						return;
					}
					else if(curKeyStateFlags==Lock_TailGate_Key)//315~433�л�
					{
						for(li=0;li<500;li++)//10s
						{
							ReadButton();
							if(curKeyStateFlags!=Lock_TailGate_Key)
							{
								LED_OFF();
								return;
							}
						}
						LED_ON();
						//�ȴ��ɿ���ǰ��ϼ�
						while(1)
						{
							//ReadButton();
							curKeyStateFlags=ReadButton_PD();
							if(curKeyStateFlags==NoKey)
							{
								_delay_ms(20);
								curKeyStateFlags=ReadButton_PD();
								if(curKeyStateFlags==NoKey)
								{
									break;
								}
							}
						}
						x=0;
						for(li=0;li<250;li++)//5s
						{
							ReadButton();
							if(x&0x01)//�������ɿ�
							{
								curKeyStateFlags=ReadButton_PD();
								if(curKeyStateFlags==NoKey)
								{
									x++;
									if(x==2)
									{
										//�ɹ�����
										RomData_ReadBytes(EEDataOriginAddr+0xf9,RomDatas, 2);
										y = RomDatas[0];
										y += RomDatas[1];
										if((RomDatas[0]==0x00) || (y!=0x00) ||(RomDatas[0]==ROM_9E))//�ڴ�״̬У��ʧ��
										{
											ChangeRF433315State(ROM_9E+1);											
										}
										else
										{
											ChangeRF433315State(ROM_9E);
										}
										for(i=0;i<10;i++)
										{
											LED_ON();
											_delay_ms(200);
											LED_OFF();
											_delay_ms(200);
										}
										break;
									}
								}
								else if(curKeyStateFlags!=UnLockKey)
								{
									LED_OFF();
									return;
								}
							}
							else//˫���Ȱ���
							{
								if(curKeyStateFlags==UnLockKey)
								{
									x++;
								}
								else if(curKeyStateFlags==NoKey)
								{
									_delay_ms(20);
								}
								else
								{
									LED_OFF();
									return;
								}
							}
						}
						LED_OFF();
						return;
					}
					LED_OFF();
					return;
				}				
			}
			if(bOnCarPower())
			{
				return;
			}
		}
	}
}
void keyRFIRTx(void)
{
	uint8_t i;
	for(i=0;i<19;i++)
		IRTxList[i]=0x00;
	for(i=0;i<4;i++)
		IRTxList[i]=0x88;
	IRTxList[4]=0x08;
	IRTxList[5]=keyValue;
	if(RomStateFlags.Bits.bRomWrited)
	{
		IRTxList[6]=ButtonTimes;
		for(i=0;i<4;i++)
			IRTxList[7+i]=SSID[i];
		for(i=0;i<8;i++)
			IRTxList[11+i]=DoorDatas[i];
	}
	IRTxCount=19;
	RFIRTxProc();
}
void keyRFTx(void)
{
	uint8_t i;
	uint8_t pre=5;	
	ATA_rfTxStartTx_C(RfTx_Config_KeyRF);//����г��fifo��xto����ɺ����ڷ���״̬��1k
	ATA_SETBITMASK_C(DFL,_BM(DFCLR))
	for(i=0;i<19;i++)
		IRTxList[i]=0x00;
	IRTxList[pre-1]=0x80;
 	ATA_rfTxFillDFifo_C(pre, IRTxList);
 	IRTxList[pre-1]=0x00;
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	IRTxList[0]=keyValue;
	if(RomStateFlags.Bits.bRomWrited)//�����ɷ�������
	{
		//д�뷢����
		IRTxList[1]=ButtonTimes;
		for(i=0;i<4;i++)
			IRTxList[2+i]=SSID[i];
		for(i=0;i<8;i++)
			IRTxList[6+i]=DoorDatas[i];
	}	
	IRTxCount=14;
	ATA_rfTxFillDFifo_C(IRTxCount, IRTxList);
	//g_sRfTx.bStatus &= ~BM_RFTXCONFIG_BSTATUS_CurFinish;
	do 
	{
		ATA_rfTxProcessing_C();		
	}while (g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_ACTIVE);
	//ATA583X_RFTxFrameProc();
}
void RFIRTxProc(void)
{	
	T3WorkType=T3_RFIRTx;
	bIRFinish_Reset();
	T3WorkType=T3_RFIRTx;
	IRTxCount=IRTxCount<<1;
	IRTxIndex=0xff;
	bIRFinish_Reset();
	ATA_POWERON_C(PRR1, PRT3)
	T3CR=0x00;
	T3CR |= _BM(T3RES);
	T3COR=60763;//��ʱ10ms�ٷ���
	T3IFR =0x07;
	T3IMR=0x02;
	T3MRA = 0x02;//xto/4
	T3MRB=0x00;
	T3CR=0x84;
	while(!bIRFinish_IsSet());
}



