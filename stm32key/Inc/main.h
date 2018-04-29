/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stm32l0xx_hal.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define I2C_SCL_Pin GPIO_PIN_14
#define I2C_SCL_GPIO_Port GPIOC
#define I2C_SDA_Pin GPIO_PIN_15
#define I2C_SDA_GPIO_Port GPIOC
#define RFIRTx_Pin GPIO_PIN_1
#define RFIRTx_GPIO_Port GPIOA
#define CarIRTx_Pin GPIO_PIN_2
#define CarIRTx_GPIO_Port GPIOA
#define IRTx_Pin GPIO_PIN_3
#define IRTx_GPIO_Port GPIOA
#define Tailgate_Pin GPIO_PIN_4
#define Tailgate_GPIO_Port GPIOA
#define Unlock_Pin GPIO_PIN_5
#define Unlock_GPIO_Port GPIOA
#define Lock_Pin GPIO_PIN_6
#define Lock_GPIO_Port GPIOA
#define FindCar_Pin GPIO_PIN_7
#define FindCar_GPIO_Port GPIOA
#define bOnCar_Pin GPIO_PIN_1
#define bOnCar_GPIO_Port GPIOB
#define bOnCar_EXTI_IRQn EXTI0_1_IRQn
#define PowerHold_Pin GPIO_PIN_2
#define PowerHold_GPIO_Port GPIOB
#define RFEn_Pin GPIO_PIN_6
#define RFEn_GPIO_Port GPIOB
#define RFData_Pin GPIO_PIN_7
#define RFData_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
//#define KeepPower

#define LeftTimesL 0
#define LeftTimesM 1
#define LeftTimesH 2


#define Verify_byteXOR

#define LED_ON() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)
#define LED_OFF() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET)

#define BATState_ON GPIO_PIN_SET
#define BATState_OFF GPIO_PIN_RESET
#define BAT_ON() HAL_GPIO_WritePin(PowerHold_GPIO_Port,PowerHold_Pin,BATState_ON)
#define BAT_OFF() HAL_GPIO_WritePin(PowerHold_GPIO_Port,PowerHold_Pin,BATState_OFF)
#define bBATON() HAL_GPIO_ReadPin(PowerHold_GPIO_Port,PowerHold_Pin)

#define bOnCarPower() HAL_GPIO_ReadPin(bOnCar_GPIO_Port,bOnCar_Pin)
#define OnCarPowerState_ON GPIO_PIN_SET
#define OnCarPowerState_OFF GPIO_PIN_RESET

#define RomData_ReadByte(Addr) wfEEPROM_ReadByte(Addr)
#define RomData_ReadBytes(nAddr,pBuf,nLen) wfEEPROM_ReadBytes(nAddr,pBuf,nLen) 

// #define RomData_WriteByte(Addr,x)   if(bBATON()==GPIO_PIN_SET) \
// 									{	\
// 										gFlags.bOldBAT=1;	\
// 									}	\
// 									else	\
// 									{	\
// 										gFlags.bOldBAT=0;	\
// 										BAT_ON();	\
// 									}	\
// 									wfEEPROM_WriteByte(Addr,x);	\
// 									if(!gFlags.bOldBAT) \	//֮ǰ���û�д򿪣���ر�
// 									{	\
// 										BAT_OFF();\
// 									}
// #define RomData_WriteBytes(nAddr,pBuf,nLen) wfEEPROM_WriteBytes(nAddr,pBuf,nLen) 

#define ROMVer 0x57
//21DF  д����05FB ѧϰ�� 15EB
#define ROM_9E 0x21
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/