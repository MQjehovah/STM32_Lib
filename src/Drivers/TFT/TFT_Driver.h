/*******************************************************************************
  * @file                   TFT_Driver.h
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.1.5
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
#ifndef __TFT_DRIVER_H
#define __TFT_DRIVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Definition ----------------------------------------------------------------*/
//#define H_VIEW
//#define USB_HARDWARE_SPI

#ifdef H_VIEW
	#define X_MAX_PIXEL	        320
	#define Y_MAX_PIXEL	        240
#else
	#define X_MAX_PIXEL	        240
	#define Y_MAX_PIXEL	        320 
#endif

#define RED   		0xf800
#define GREEN  		0x07e0
#define BLUE	    0x001f
#define WHITE	    0xffff
#define BLACK	    0x0000
#define YELLOW      0xFFE0
#define GRAY0       0xEF7D      	//灰色0 3165 00110 001011 00101
#define GRAY1       0x8410      	//灰色1      00000 000000 00000
#define GRAY2       0x4208      	//灰色2  1111111111011111

#define TFT_CTRL   	  	GPIOB		//定义TFT数据端口为PB组
#define TFT_CS        	GPIO_Pin_11 //MCU_PB11			对应接液晶屏(或者模块)TFT --CS
#define TFT_RST     	GPIO_Pin_12	//MCU_PB12			对应接液晶屏(或者模块)TFT --RST
#define TFT_RS         	GPIO_Pin_10	//MCU_PB10			对应接液晶屏(或者模块)TFT --RS/DC
#define TFT_SDA        	GPIO_Pin_15	//MCU_PB15 MOSI	    对应接液晶屏(或者模块)TFT --SDA
#define TFT_SCL        	GPIO_Pin_13	//MCU_PB13 MISO		对应接液晶屏(或者模块)TFT --SCL
//#define TFT_LED        	GPIO_Pin_9  //MCU_PB9   		对应接液晶屏(或者模块)TFT --PIN_LED背光正极
#define TFT_SDO        	GPIO_Pin_14	//MCU_PB14 MISO	对应接液晶屏(或者模块)TFT --SDO 

//液晶控制口置1操作语句宏定义
#define	TFT_CS_SET  	TFT_CTRL->BSRR=TFT_CS    
#define	TFT_RS_SET  	TFT_CTRL->BSRR=TFT_RS    
#define	TFT_SDA_SET  	TFT_CTRL->BSRR=TFT_SDA    
#define	TFT_SCL_SET  	TFT_CTRL->BSRR=TFT_SCL    
#define	TFT_RST_SET  	TFT_CTRL->BSRR=TFT_RST    
#define	TFT_LED_SET  	TFT_CTRL->BSRR=TFT_LED    

//液晶控制口置0操作语句宏定义
#define	TFT_CS_CLR  	TFT_CTRL->BRR=TFT_CS    
#define	TFT_RS_CLR  	TFT_CTRL->BRR=TFT_RS    
#define	TFT_SDA_CLR  	TFT_CTRL->BRR=TFT_SDA    
#define	TFT_SCL_CLR  	TFT_CTRL->BRR=TFT_SCL    
#define	TFT_RST_CLR  	TFT_CTRL->BRR=TFT_RST    
#define	TFT_LED_CLR  	TFT_CTRL->BRR=TFT_LED  
/* Exported Functions --------------------------------------------------------*/
void TFT_Init(void);
void TFT_Clear(u16 Color);
void TFT_SetXY(u16 x,u16 y);
void TFT_DrawPoint(u16 x,u16 y,u16 Data);
u16 TFT_ReadPoint(u16 x,u16 y);

#endif
/*********************************END OF FILE**********************************/
