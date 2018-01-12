#include "HardwareProfile.h"
__CONFIG(1,IESO_OFF&FCMEN_OFF&FOSC_INTIO67);
__CONFIG(2,BORV_190&BOREN_OFF&PWRTEN_ON&WDTPS_32768&WDTEN_OFF);//看门狗2s复位
__CONFIG(3,MCLRE_INTMCLR&P2BMX_PORTC0&HFOFST_OFF&CCP3MX_PORTC6&PBADEN_OFF&CCP2MX_PORTC1);
__CONFIG(4,DEBUG_OFF&XINST_OFF&LVP_OFF&STVREN_OFF);
__CONFIG(5,CP1_ON&CP0_ON&CPD_ON&CPB_ON);
__CONFIG(6,WRT1_ON&WRT0_ON&WRTD_OFF&WRTB_ON&WRTC_OFF);
__CONFIG(7,EBTR1_OFF&EBTR0_OFF&EBTRB_OFF);

__IDLOC(11111111);

__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//00
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//01
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//02
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//03
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//04
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//05
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//06
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//07
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);__EEPROM_DATA(0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01);//08
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xFF, 0x02, 0xFE, 0x00, 0x51, 0x68, 0x21, 0xDF);//09
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//0a
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//0b
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//0c
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//0d,psw
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//0e
__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);__EEPROM_DATA(0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff);//0f

void InitSystem(void);
void CHK_SUPPLY(void)
{
	if(PW_IN)
	{
		f20.power=1;
	}
	__delay_20ms(1);
	if(!PW_IN)
		f20.power=0;
}
unsigned char ww[16];
void main(void)
{
	unsigned char i,j;
	InitSystem();	
// 	for(j=0;j<16;j++)
// 	{
// 		for(i=0;i<16;i++)
// 		{
// 			ww[i]=0xff;//j*16+i;			
// 		}
// 		M24XX_Write(ww,16,j*16);
// 	}
//  	for(j=0;j<16;j++)
//  	{
//  		for(i=0;i<16;i++)
//  		{
//  			ww[i]=0;			
//  		}
//  		M24XX_Read(ww,16,j*16);
//  	}
// 	for(i=0;i<8;i++)
// 		wfTempList[i]=i;	
	while(1)
	{		
		CLRWDT();
		CHK_SUPPLY();
		if(f20.power)
		{
			POW_CAR();
		}
		else
		{
			POW_BAT();
		}
	}
}
void interrupt low_priority lowISR(void)
{
	if(CCP1IE && CCP1IF)
	{
		CCP1IF=0;
		TMR3ON=0;
		TMR3L=0;
		TMR3H=0;
		TMR3ON=1;
		f20.rxok=1;

		if(f20.ONcal)
		{
			wfTemp16.u8H=CCPR1H;
			wfTemp16.u8L=CCPR1L;
			if(wfTemp16.u16>=0x03e0)//994
			{
// 				RLCF	TEMP1,F,A		;
// 				RLCF	TEMP0,F,A		;
// 				RLCF	TEMP1,F,A		;
// 				RLCF	TEMP0,W,A		;
// 				;;	MOVF	TEMP0,W,A		;
// 				MOVWF	RAM_A4,BANKED	
			}
		}
		else
		{
			f20.ONcal=1;
			f21.over1=0;
			//2ms check
			TMR1ON=0;
			TMR1L=0x2f;
			TMR1H=0xf8;
			TMR1ON=1;			
		}		
	}
	if(TMR1IE && TMR1IF)
	{
		TMR1IE=0;
		TMR1IF=0;
		if(f20.ONbuzh)//打开捕捉后才进行超时检测
		{
			if(f20.ONcal)//打开计算后，才进行检查，超时后关闭捕捉
			{
				CCP1IF=0;
				CCP1CON=0;
				f20.rxok=1;//收到标志
				f21.over1=1;//超时后，外部查询标志
			}
		}
	}
	if(RBIE && RBIF)
	{
		IOCB=0;		
		RBIE=0;
		RBIF=0;
		PEIE=0;
		GIE=0;

//		BSF	RAM_CB,5,BANKED		;
	}
	if(HLVDIE && HLVDIF)
	{
		BAT_CL=1;
		HLVDIE=0;
		HLVDIF=0;
		PEIE=0;
		GIE=0;
	}
}
