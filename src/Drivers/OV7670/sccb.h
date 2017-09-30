/*******************************************************************************
  * @file                   sccb.h
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.2.26
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
#ifndef __SCCB_H
#define __SCCB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Definition ----------------------------------------------------------------*/
#define SCCB_SIC_Port    GPIOB
#define SCCB_SIC_Pin     GPIO_Pin_5	

#define SCCB_SID_Port    GPIOB 
#define SCCB_SID_Pin     GPIO_Pin_6 

#define SCCB_SDA_IN      SCCB_SID_GPIO_INPUT()
#define SCCB_SDA_OUT     SCCB_SID_GPIO_OUTPUT()

//IO��������	 
#define SCCB_SIC_H       GPIO_SetBits(SCCB_SIC_Port,SCCB_SIC_Pin)	              //GPIOA->BSRR = SCCB_SIC_BIT;  �Զ˿�A�ض�λ������	,�������ø�
#define SCCB_SIC_L       GPIO_ResetBits(SCCB_SIC_Port,SCCB_SIC_Pin)             //GPIOA->BRR =  SCCB_SIC_BIT;  �������õ�

#define SCCB_SID_H       GPIO_SetBits(SCCB_SID_Port,SCCB_SID_Pin)	              //GPIOA->BSRR = SCCB_SID_BIT;  
#define SCCB_SID_L       GPIO_ResetBits(SCCB_SID_Port,SCCB_SID_Pin)             //GPIOA->BRR =  SCCB_SID_BIT;

#define SCCB_READ_SID    GPIOB->IDR&0x40                                    //    GPIO_ReadInputDataBit(SCCB_SID_Port,SCCB_SID_Pin) ����SDA    

#define SCCB_ID   			 0X42  			        //OV7670��ID
/* Exported Functions --------------------------------------------------------*/
void SCCB_SID_GPIO_OUTPUT(void);
void SCCB_SID_GPIO_INPUT(void);

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);

u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);

#endif
/*********************************END OF FILE**********************************/










