//HI-TECH C PRO for the PIC18 MCU Family  V9.63PL3
#include "HardwareProfile.h"

//__CONFIG(HS&WDTEN&PWRTEN&MCLREN&BOREN);
// __CONFIG(INTIO&
// 			WDTEN&
// 			PWRTEN&
// 			MCLREN&
// 			BOREN&
// 			CP&
// 			IESODIS&
// 			FCMDIS);

__CONFIG(FOSC_INTOSC&
	WDTE_NSLEEP&
	PWRTE_ON &
	MCLRE_ON&
	CP_ON&
	CPD_OFF&
	BOREN_ON&
	CLKOUTEN_OFF&
	IESO_OFF&
	FCMEN_OFF);
__CONFIG(WRT_ALL&
	PLLEN_ON&
	STVREN_ON&
	BORV_25&
	LVP_ON);

__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//00,k0
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//01,k1
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//02,k2
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//03,k3
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//04,k4
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//05,k5
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//06,k6
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//07,k7
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//08,del
__EEPROM_DATA(0x22,0xff,0x00,0x00,0xf0,0x14,0x08,0x00);__EEPROM_DATA(0x20,0x45,0x45,0x81,0x32,0x28,0x07,0x30);//09
__EEPROM_DATA(0x06,0x05,0x00,0x01,0x00,0x10,0x07,0x01);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//0a
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x30,0x34,0x53,0x36,0x60,0x42,0x11,0x01);//0b
__EEPROM_DATA(0xc2,0xe9,0x01,0xf0,0xed,0xba,0x7c,0x5c);__EEPROM_DATA(0x00,0x00,0x00,0xe0,0x90,0x20,0x00,0x00);//0c
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//0d,psw
__EEPROM_DATA(0x45,0x21,0x22,0x04,0x14,0x00,0x00,0x00);__EEPROM_DATA(0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x05);//0e
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff);//0f

const unsigned char commandHeader[]=
{
	0x76,0x52,0x15,0x96,0xa0,0x9a,0x42,0xff
};
void GotoSleep(void)
{
	CREN=1;
	TMR1ON=0;
	WUE=1;
	SLEEP();
	NOP();
	NOP();
	RESET();
}
void main()
{
	unsigned char i,sum;
	InitSystem();	
	while(1)
	{	
//		Uart1_PutChar(i++);
 		if(gFlags.bSleep)
 		{
 			GotoSleep();
 		}
		if(gFlags.bRx)
		{
			sum=GetVerify_Sum(uartRxList,10);
			if(sum==uartRxList[10])
			{
				if((uartRxList[0]&0xf0)==0x30)
				{
// 					if(eeprom_read(0xff)==0xff)//从未写入过
// 					{
// 						uartRxCount=0;
// 						gFlags.bRx=0;
// 						continue;
// 					}
					ProcCommand_work_0x3x();
				}
				else if(uartRxList[0]==0x76)//读数据	
				{
					ProcCommand_read_0x76();	
				}
				else if(uartRxList[0]==0x52)//renew
				{
					ProcCommand_renew_0x52();
				}
				else if(uartRxList[0]==0x15)//写启动
				{
					ProcCommand_write_0x15();
				}
				else if(uartRxList[0]==0x96)
				{
					ProcCommand_write_0x96();
				}
				else if(uartRxList[0]==0xa0)
				{
					ProcCommand_write_0xa0();	
				}
				else if(uartRxList[0]==0x9a)
				{
					ProcCommand_write_0x9a();
				}
				else if(uartRxList[0]==0x42)
				{
					ProcCommand_lock_0x42();
				}
				else
				{
					for(i=0;i<8;i++)
					{
						uartTxList[i+2]=0;
					}
				}
				uartTxList[10]=GetVerify_Sum(uartTxList,10);
				if (gFlags.bAlarm)
				{
					for(i=0;i<100;i++)
					{
						CLRWDT();
						LATA2=0;
						__delay_ms(2);
						LATA2=1;
						__delay_ms(2);
					}
					
				}
				else
					__delay_ms(10);
				Uart1_PutChars(uartTxList,11);
				while(Uart1_TRMT==0);
				//__delay_ms(4);
			}			
			GotoSleep();
		}
	}
}
void interrupt lowISR(void)
{
	unsigned char i;
	if(RCIE&&RCIF)
	{
		RCIF=0;
		if(OERR==1)
		{
			CREN=0;
			CREN=1;
		}
		WRITETIMER1(T1_1ms);
		T1Times=0;
		i=RCREG;
		if(gFlags.bRx==0)
		{
			uartRxList[uartRxCount++]=i;
			if(uartRxCount==uartRxListMax)
			{
				CREN=0;
				gFlags.bRx=1;
				TMR1ON=0;
			}	
			else if(uartRxCount==1)
			{
				for(i=0;;i++)
				{
					if(commandHeader[i]==0xff)
					{
						i=0xfe;
						break;
					}
					if(uartRxList[0]==commandHeader[i])
					{
						i=0xff;
						break;
					}					
				}
				if(i==0xfe)
				{
					if((uartRxList[0]&0xf0)!=0x30)
						uartRxCount=0;
				}
			}
		}
		//ATCommand_ProcRx(rx);
	}
	if(TMR1IF&& TMR1IE)
	{
		TMR1IF=0;
		WRITETIMER1(T1_1ms);	
		uartRxCount=0;
		T1Times++;
		if(T1Times>20)
			gFlags.bSleep=1;
	}	
}
void bSameHash(const unsigned char* d1,const unsigned char* d2)
{
	unsigned char i;	
	for(i=0;i<8;i++)
	{
		if(d1[i]!=d2[i])
		{
			gFlags.bSame=0;
			return;
		}
	}
	gFlags.bSame=1;
}
void bSameHash_eep(const unsigned char* d1,unsigned char addr)
{
	unsigned char i,eep;	
	for(i=0;i<8;i++)
	{
		eep=eeprom_read(addr+i);
		if(d1[i]!=eep)
		{
			gFlags.bSame=0;
			return;
		}
	}
	gFlags.bSame=1;
}
