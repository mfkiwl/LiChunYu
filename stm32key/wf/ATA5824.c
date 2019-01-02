#include "ATA5824.h"
#include "SimSPI.h"
#include "..\Inc\tim.h"
#include "Variables.h"
#include "wfSys.h"
#include "AS3933.h"
#include "Function.h"
#include "Verify.h"
#include "wfEEPROM.h"
_ATA5824_ICDataStruct ATA5824_ICData[ATA5824_ICData_BufLen];
uint8_t ATA5824_TxList[20];
uint32_t ATA5824_TxCount;
uint32_t ATA5824_RxTick;
uint8_t ATA5824_RxList[20];
uint32_t ATA5824_RxCount;
uint32_t ATA5824_ICDataIndex;

uint8_t keyIndex=0x70;
uint8_t keyCount=0;
#define TMDODelay() wfDelay_us(43)
void ATA5824_TxFrameProc(void);
void ATA5824_Test(void)
{
	uint8_t i;
	HAL_GPIO_WritePin(ATA5824_PWR_ON_GPIO_Port,ATA5824_PWR_ON_Pin,GPIO_PIN_SET);
	ATA5824_Init();
	while(1)
	{
		gFlags.ATA5824_bRxFrame=0;
		ATA5824_RxStart();
		while(!gFlags.ATA5824_bRxFrame);
		ATA5824_IDLE();
		ATA5824_FrameProc();//�������
		wfDelay_ms(2);
	}
// 	for(i=0;i<8;i++)
// 		ATA5824_TxList[i]=i;
// 	ATA5824_TxCount=8;
// 
// 	while(1)
// 	{
// 		ATA5824_TxStart();
// 		wfDelay_ms(1);
// 		ATA5824_TxFrameProc();
// 		ATA5824_IDLE();
// 		wfDelay_ms(1000);
// 	}
	while(1);
}


void ATA5824_Exit(void)
{
	AS3933_Init();
	BAT_OFF();
	while(1);
}
void ATA5824_WaitRx(uint32_t timeOut)
{
	uint8_t i,addr,key;	
	uint32_t nDelay;
	ATA5824_RxStart();
	GetKeyParam();
	key=SSID[0]&0x07;
	while(1)//��1֡:SSID
	{
		if(timeOut!=-1)
		{
			if(GetDeltaTick(ATA5824_RxTick)>timeOut)//��ʱ���˳�
			{
				return;
				//ATA5824_Exit();
			}
		}
		if(gFlags.ATA5824_bRxFrame)
		{
			ATA5824_IDLE();
			ATA5824_FrameProc();//�������
			if(ATA5824_RxCount==5)
			{
				if((ATA5824_RxList[1]&0xf8)==(SSID[0]&0xf8) &&
					ATA5824_RxList[2]==(SSID[1]) &&
					ATA5824_RxList[3]==(SSID[2]))
				{
					if(ATA5824_RxList[0]==0x4a ||ATA5824_RxList[0]==0x4e||ATA5824_RxList[0]==0x49)//Ѱ��Կ��
			        {						
						nDelay=14500+key*3500;
						wfDelay_us(nDelay);//14ms 78,  17ms 71, 21ms 6a,25ms 63,29ms 5c,33ms 55,37ms 4e
						ATA5824_TxList[0]=0x78-key*7;//keyIndex;
						ATA5824_TxCount=1;
						ATA5824_TxFrameProc();
						wfDelay_ms(20);
						ATA5824_RxStart();
					}
					else if(ATA5824_RxList[0]==0x4d)//�Ű���
					{
						ATA5824_TxList[0]=0xf2;
						ATA5824_TxList[1]=0x59;
						if(ATA5824_RxList[4]==0x91 || ATA5824_RxList[4]==0x90)//����
						{
							GetDoorProc(0x23);
						}
						else if(ATA5824_RxList[4]==0x89 || ATA5824_RxList[4]==0x88)//����
						{
							GetDoorProc(0x21);
						}
						else
						{
							ATA5824_RxStart();
						}
						wfDelay_us(8000);
						ATA5824_TxList[2]=ButtonTimes;//��������
						for(i=0;i<8;i++)
							ATA5824_TxList[3+i]=DoorDatas[i];
						ATA5824_TxList[11]=GetVerify_Sum(ATA5824_TxList,11);
						ATA5824_TxCount=12;
						ATA5824_TxFrameProc();
						/************************************************************************/
						/*  �޸İ�������                                                        */
						/************************************************************************/
						addr=LeftTimes69&0x03;
						addr=addr+0x90;
						ButtonTimes=RomData_ReadByte(addr);
						ButtonTimes++;
						RomData_WriteByte(addr,ButtonTimes);
						/************************************************************************/
						/*                                                                      */
						/************************************************************************/
						wfDelay_ms(10);
						ATA5824_RxStart();
					}
					else
					{
						ATA5824_RxStart();
					}
				}
				else
				{
					ATA5824_RxStart();
				}
			}
			else if(ATA5824_RxCount==10)
			{
				if(ATA5824_RxList[0]==(0x50+key))
				{
					wfDelay_us(1400);
					ATA5824_TxList[0]=0x5c;
					ATA5824_TxList[1]=LeftTimes69;//LeftTimes[LeftTimesL];
					ATA5824_TxList[2]=LeftTimes[LeftTimesM];
					ATA5824_TxList[3]=LeftTimes[LeftTimesH];
					ATA5824_TxList[4]=GetVerify_Sum(ATA5824_TxList,4);
					ATA5824_TxCount=5;
					ATA5824_TxFrameProc();

					GetKeyWorkValue(&ATA5824_RxList[1],0x50);	
					for(i=0;i<8;i++)
					{
						ATA5824_TxList[1+i]=WorkValueDatas[i];
					}
					ATA5824_TxList[0]=0x27;					
					ATA5824_TxList[9]=GetVerify_Sum(ATA5824_TxList,9);
					ATA5824_TxCount=10;
					ATA5824_TxFrameProc();
					UsedDEC();
					for(i=0;i<4;i++)
						RomDatas[i]=0x01;
					RomData_WriteBytes(0x90,RomDatas,4);
					GetKeyState();
					GetKeyParam();//���Կ�׵�ǰ�������
					return;
				}
				else
				{
					ATA5824_RxStart();
				}
			}
			else
			{
				ATA5824_RxStart();
			}
		}
	}	
}
void ATA5824_TxFrameProc(void)
{
	uint32_t i,j;
	uint8_t x;
	uint32_t Pre0=12;
	ATA5824_TxStart();
	wfDelay_us(500);
	for(i=0;i<Pre0;i++)
	{
		SIMSPI_SDO_High();
		TMDODelay();
		SIMSPI_SDO_Low();
		TMDODelay();
	}
	SIMSPI_SDO_Low();
	TMDODelay();
	SIMSPI_SDO_High();
	TMDODelay();
	for(i=0;i<ATA5824_TxCount;i++)
	{
		x=ATA5824_TxList[i];
		for (j=0;j<8;j++)//�ȵ�λ
		{
			if((x&0x01)==0x01)
			{
				SIMSPI_SDO_Low();
				TMDODelay();
				SIMSPI_SDO_High();
				TMDODelay();
			}
			else
			{
				SIMSPI_SDO_High();
				TMDODelay();
				SIMSPI_SDO_Low();
				TMDODelay();
			}
			x=x>>1;
		}
	}
	SIMSPI_SDO_Low();
	TMDODelay();
	SIMSPI_SDO_High();
	TMDODelay();
	SIMSPI_SDO_Low();
	TMDODelay();
	ATA5824_IDLE();
}
void ATA5824_FrameProc(void)
{
	uint32_t i,x;	
	uint8_t rxBitList[500],rxBitList1[500];
	uint32_t rxBitLen=0,rxBitLen1=0,nBit;
	uint32_t bData=0,bOdd=0;	
	ATA5824_RxCount=0;
	for(i=0;i<ATA5824_ICDataIndex;i++)
	{
		rxBitList[rxBitLen++]=ATA5824_ICData[i].pinState;
		if(ATA5824_ICData[i].time>60)//ȫλ������������λֵ
		{
			rxBitList[rxBitLen++]=ATA5824_ICData[i].pinState;
			if(bData==0)//�жϳ���ż����
			{
				if(i%2==0)
				{
					bOdd=1;//������
				}
				else
				{
					bOdd=0;//ż����
				}
				bData=1;
			}
		}
	}
	for(i=0;i<rxBitLen;i+=2)
	{
		if(bOdd==0)//ż����
		{
			rxBitList1[rxBitLen1++]=rxBitList[i];
		}
		else
		{
			rxBitList1[rxBitLen1++]=rxBitList[i+1];
		}
	}
	bData=0;
	nBit=0;
	for(i=0;i<rxBitLen1;i++)
	{
		if(bData==0)//ȥ��ͬ��ͷ
		{
			if(rxBitList1[i]==1)
			{
				bData=1;
			}
		}
		else
		{
			if(rxBitList1[i]==1)
			{
				ATA5824_RxList[ATA5824_RxCount] |= 0x80;
			}
			else
			{
				ATA5824_RxList[ATA5824_RxCount] &= 0x7f;
			}
			nBit++;
			if(nBit>=8)
			{
				ATA5824_RxCount++;
				nBit=0;
			}
			else
			{
				ATA5824_RxList[ATA5824_RxCount] =ATA5824_RxList[ATA5824_RxCount] >>1;
			}
		}
	}
	//֡������ɣ��ȴ�����
	//ATA5824_RxStart();
}
void ATA5824_ICProc(void)
{
	uint32_t read;
 	if(gFlags.ATA5824_bFirstIC!=0)//��һ�β�׽
 	{
 		gFlags.ATA5824_bFirstIC=0;
 		htim22.Instance->CNT=0;
 		__HAL_TIM_CLEAR_IT(&htim22, TIM_IT_UPDATE);
 		HAL_TIM_Base_Start_IT(&htim22);	
 	}
 	else
 	{
 		read=HAL_TIM_ReadCapturedValue(&htim22,TIM_CHANNEL_2);//+13;
 		htim22.Instance->CNT=0;
 		ATA5824_ICData[ATA5824_ICDataIndex].time=read;
 		ATA5824_ICData[ATA5824_ICDataIndex].pinState=HAL_GPIO_ReadPin(ATA5824_MISO_TMDI_GPIO_Port,ATA5824_MISO_TMDI_Pin);
 		ATA5824_ICDataIndex++;		
 	}
}
void ATA5824_RxStart(void)
{
	gFlags.ATA5824_bRxFrame=0;	
	gFlags.ATA5824_bFirstIC=1;
	ATA5824_ICDataIndex=0;
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);
	SimSPI_Proc(0x60);
	SimSPI_Proc(0x25);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
 	__HAL_TIM_CLEAR_IT(&htim22, TIM_IT_UPDATE);
 	HAL_TIM_IC_Start_IT(&htim22, TIM_CHANNEL_2);	
	ATA5824_RxTick=HAL_GetTick();
}
void ATA5824_TxStart(void)
{
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);
	SimSPI_Proc(0x60);
	SimSPI_Proc(0x23);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
}
void ATA5824_IDLE(void)
{
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);
	SimSPI_Proc(0x60);
	SimSPI_Proc(0x21);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
}
void ATA5824_Init(void)
{
	uint8_t rx;
	HAL_GPIO_WritePin(ATA5824_PWR_ON_GPIO_Port,ATA5824_PWR_ON_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SimSPI_Init();
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);

 	rx=SimSPI_Proc(0x60);
 	rx=SimSPI_Proc(0x21);
 
 	rx=SimSPI_Proc(0x61);
 	rx=SimSPI_Proc(0xad);//(0xb1);
 
 	rx=SimSPI_Proc(0x62);
 	rx=SimSPI_Proc(0x78);//(0x7b);
 
 	rx=SimSPI_Proc(0x63);
 	rx=SimSPI_Proc(0x00);
 
 	rx=SimSPI_Proc(0x64);
 	rx=SimSPI_Proc(0x9b);
 
 	rx=SimSPI_Proc(0x65);
 	rx=SimSPI_Proc(0xfa);
 
 	rx=SimSPI_Proc(0x66);
 	rx=SimSPI_Proc(0xa8);
 
 	rx=SimSPI_Proc(0x67);
 	rx=SimSPI_Proc(0x50);

	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
}

void ATA5824_RFInit(void)
{
	uint8_t rx;
	HAL_GPIO_WritePin(ATA5824_PWR_ON_GPIO_Port,ATA5824_PWR_ON_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SimSPI_Init();
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);

	rx=SimSPI_Proc(0x60);
	rx=SimSPI_Proc(0x21);

	rx=SimSPI_Proc(0x61);
	rx=SimSPI_Proc(0xb1);//(0xb1);

	rx=SimSPI_Proc(0x62);
	rx=SimSPI_Proc(0x7b);//(0x7b);

	rx=SimSPI_Proc(0x63);
	rx=SimSPI_Proc(0x00);

	rx=SimSPI_Proc(0x64);
	rx=SimSPI_Proc(0x9b);

	rx=SimSPI_Proc(0x65);
	rx=SimSPI_Proc(0xfa);

	rx=SimSPI_Proc(0x66);
	rx=SimSPI_Proc(0xa8);

	rx=SimSPI_Proc(0x67);
	rx=SimSPI_Proc(0x50);

	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
}