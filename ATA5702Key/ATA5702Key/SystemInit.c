#include "HardwareProfile.h"
#include "firmware/globals.h"
#include "firmware/LF_RX.h"
#include "RF_TX.h"
void SystemInit(void)
{
	/************************************************************************/
	/* PORTD
	0:power_hold��	1
	1:IR RX1	��	������	INT0
	2:LED_Red	��	1	led��
	3:S1_LOCK	��	����
	4:S2_UNLOCK	��	����
	5:IR_RX2	��	������	TICP
	6:S3_Tail	��	����
	7:S4_Find	��	����                                                    */
	/************************************************************************/
	PORTD=0xff;
	DDRD =0x05;
	/************************************************************************/
	/* PORTC
	0:CAR_IR_TX	��	1
	1:RF_IR_TX	��	1
	2:OnCar_INT	��	������			                                         */
	/************************************************************************/
	PORTC=0x03;
	DDRC =0x03;
	/************************************************************************/
	/* PORTB
	7:bootloader	��	����                                                */
	/************************************************************************/
	PORTB=0xFF;
	DDRB =0x00;
	MCUCR |= _BM(ENPS);//ʹ�ܶ˿��趨
	MCUCR &= ~_BM(PUD);	
	ATA_globalsClkStartXTO_C();//ʹ��frc��ΪϵͳƵ��,6.07625Mhz
	
	//events_reset=MCUSR;//��¼��λԭ��
	MCUSR=0x00;//�����λԴ��־
	ATA_globalsWdtDisable_C();//��ʼ���׶ν��ÿ��Ź�
	//�����Ƶ�ر����ʼ����Ƶģ��
 	if ( (LFCPR & _BM(LFCALRY)) == 0x00U ) //(1)
     {
         ATA_lfRxInit_C();
         LFCPR = _BM(LFCPCE);
         LFCPR = _BM(LFCALRY);    
     }
 	else
 	{
 		ATA_POWERON_C(PRR1, PRLFR);//��ʼ��
 		ATA_POWERON_C(PRR1, PRLFPH);//��ƵЭ�鴦��ģ�鹩��
 	}	
	ATA_calibInit_C();	
	//ATA_globalsWdtEnable_C(0x1B);//32K cycles 8*32K = 256K cycles 2.1s 1.75s
	/* Enable global interrupts */
	_SEI;	

	g_sRfTx.bFlags      = 0x00U;
	g_sRfTx.bTuneFlags  = 0x00U;
	g_sRfTx.bStatus     = 0x00U;
	g_sRfTx.bConfig     = 0x00U;
	g_sRfTx.pAddress   = 0x0000U;
}


