#include "HardwareProfile.h"
void InitMCU(void)             //系统初始化程序
{
	OSCCON=0b11110000;//16Mhz
	while(!HFIOFR);	
#ifdef protues
	ANSELA=0;
	ANSELB=0;
	APFCON0=0x00;
	APFCON1=0x01;
#else
	ANSELA=0;
	APFCON=0b10000100;
	TRISA2=0;
	LATA2=1;
#endif

	/************************************************************************/
	/* 串口1																*/
	/************************************************************************/
	BAUDCON=0b00001000;	
	SPBRGH=0x03;//Baud Rate = Fosc/(4(SPBRG+1)) = 9600
	SPBRGL=0x40;
	TXSTA=0b00100110;//异步方式、使能发送(bit6=1是9位数据)八位字长、高波特率(bit2低速0)
	RCSTA=0b10010000; //使能串口、使能接收、异步方式、(bit6=1是9位数据)八位字长	
	RCIE=1;//使能中断
	RCIF=0;//清中断标志

	//AD初始化	
// 	ADCON0=0b10011000;//选择AN6
// 	ADCON1=0b00100000;  //fosc/64
// 	ANSEL=0;//RA2为模拟输入
// 	ANSELH=0;
// 	ANS4=1;//贴片检测
// 	ANS6=1;//电池电量
// 	TRISC0=1;
// 	TRISC2=1;
// 	ADON=0;
// 	OPTION=0b00001111;//看门狗预分频128	
	/************************************************************************/
	/* IO初始化                                                             */
	/************************************************************************/
// 	TRISA2=0;
// 	TRISA1=0;
// 	TRISA0=0;
// 	LCDBG=0;
// 	LeftRe=0;
// 	RightRe=0;
// 	TRISC=0x07;
	/************************************************************************/
	/* TMR1 系统节拍，100ms                                                 */
	/************************************************************************/
 	T1CON=0b00110000;//1:8,8us
 	WRITETIMER1(T1_1ms);//100ms
 	TMR1ON=1;
 	TMR1IE=1;//使能中断
 	TMR1IF=0;//清中断标志
	/************************************************************************/
	/* 电平变化中断                                                         */
	/************************************************************************/
// 	IOCB=0x70;
// 	RBIE=1;
// 	RBIF=0;	
	PEIE=1;
	GIE=1;
}
void InitVariable(void)
{
	gFlags.AllFlag=0;
	uartRxCount=0;
	T1Times=0;
}
void InitSystem( void )
{
	InitMCU();
	
}
