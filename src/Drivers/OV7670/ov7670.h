/*******************************************************************************
  * @file                   OV7670.h
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.2.26
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
#ifndef _OV7670_H
#define _OV7670_H

/* Includes ------------------------------------------------------------------*/
#include "sccb.h"
/* Definition ----------------------------------------------------------------*/
#define OV7670_VSYNC_Port 	 GPIOB					                     
#define OV7670_VSYNC_Pin     GPIO_Pin_0 

#define OV7670_OE_Port 	     GPIOB					                     
#define OV7670_OE_Pin        GPIO_Pin_5 

#define OV7670_WEN_Port 	   GPIOB					                     
#define OV7670_WEN_Pin       GPIO_Pin_6 

#define OV7670_WRST_Port 	   GPIOB					                     
#define OV7670_WRST_Pin      GPIO_Pin_7

#define OV7670_RRST_Port 	   GPIOB					                     
#define OV7670_RRST_Pin      GPIO_Pin_8

#define OV7670_RCLK_Port 	   GPIOB					                     
#define OV7670_RCLK_Pin      GPIO_Pin_9

#define OV7670_DATA_Port 	   GPIOA					                     
#define OV7670_DATA_Pin      0x00ff

#define OV7670_OE_H			GPIO_SetBits(OV7670_OE_Port,OV7670_OE_Pin)    		        //片选信号(OE)
#define OV7670_OE_L			GPIO_ResetBits(OV7670_OE_Port,OV7670_OE_Pin) 

#define OV7670_WEN_H		GPIO_SetBits(OV7670_WEN_Port,OV7670_WEN_Pin)					    //写入FIFO使能
#define OV7670_WEN_L		GPIO_ResetBits(OV7670_WEN_Port,OV7670_WEN_Pin)		

#define OV7670_WRST_H		GPIO_SetBits(OV7670_WRST_Port,OV7670_WRST_Pin)					  //写指针复位
#define OV7670_WRST_L		GPIO_ResetBits(OV7670_WRST_Port,OV7670_WRST_Pin)					

#define OV7670_RRST_H		GPIO_SetBits(OV7670_RRST_Port,OV7670_RRST_Pin)            //读指针复位
#define OV7670_RRST_L		GPIO_ResetBits(OV7670_RRST_Port,OV7670_RRST_Pin)

#define OV7670_RCLK_H		GPIO_SetBits(OV7670_RCLK_Port,OV7670_RCLK_Pin)		        //设置读数据时钟高电平
#define OV7670_RCLK_L		GPIO_ResetBits(OV7670_RCLK_Port,OV7670_RCLK_Pin)		

#define OV7670_DATA  	  GPIOA->IDR&0x00FF  					              //数据输入端口
/* Exported Functions --------------------------------------------------------*/    				 
u8   OV7670_Init(void);		  	   		 
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void OV7670_GPIO_Init(void);

#endif
/*********************************END OF FILE**********************************/
