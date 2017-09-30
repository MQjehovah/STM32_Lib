/*******************************************************************************
  * @file                   ov7670.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.2.28
  * @brief                  本程序参考了ALIENTEK战舰STM32开发板V3相关资料
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "ov7670.h"
#include "ov7670cfg.h"	  
#include "delay.h"			 
#include "sccb.h"	
#include "exti.h"
#include "USART_Driver.h"

#include <stdlib.h>
/* Definition ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  初始化OV7670              
  * @param  None              
  * @retval None              
  * @Note   返回0:成功  
            返回其他值:错误代码	
*******************************************************************************/
u8 OV7670_Init(void)
{
	u8 temp;
	u16 i=0;	

//	OV7670_GPIO_Init();                      //初始化IO
 	SCCB_Init();        		                 //初始化SCCB 的IO口	 
	
#ifdef debug
	printf("SCCB初始化成功\n");
#endif
	
 	if(SCCB_WR_Reg(0x12,0x80))return 1;	     //复位SCCB	 
	
#ifdef debug
	printf("复位SCCB成功\n");
#endif	
	
	Delay_ms(50);  	
 	temp=SCCB_RD_Reg(0x0b);                  //读取产品型号
	
#ifdef debug
	printf("读取成功\n");
	printf("temp=%d\n",temp);
#endif
	
	if(temp!=0x73)return 2;  
 	temp=SCCB_RD_Reg(0x0a);   
	if(temp!=0x76)return 2;
	for(i=0;i<sizeof(ov7670_init_reg_tbl)/sizeof(ov7670_init_reg_tbl[0]);i++)  //初始化序列	  
	{
	   	SCCB_WR_Reg(ov7670_init_reg_tbl[i][0],ov7670_init_reg_tbl[i][1]);
  	}
   	return 0x00; 	                         //ok
} 
/*******************************************************************************
  * @brief  设置窗口           
  * @param  None              
  * @retval None              
  * @Note   None	
*******************************************************************************/
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp; 
	endx=sx+width*2;	                    //V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				        //读取Vref之前的值
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				        //设置Vref的start和end的最低2位
	SCCB_WR_Reg(0X19,sx>>2);			        //设置Vref的start高8位
	SCCB_WR_Reg(0X1A,endx>>2);		       	//设置Vref的end的高8位

	temp=SCCB_RD_Reg(0X32);			        	//读取Href之前的值
	temp&=0XC0; 
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X17,sy>>3);			        //设置Href的start高8位
	SCCB_WR_Reg(0X18,endy>>3);			      //设置Href的end的高8位
}
/*******************************************************************************
  * @brief  GPIO初始化           
  * @param  None              
  * @retval None              
  * @Note   None	
*******************************************************************************/
void OV7670_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //使能相关端口时钟
 
	GPIO_InitStructure.GPIO_Pin = OV7670_VSYNC_Pin; 	                                               //VSYNC 输入 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(OV7670_VSYNC_Port, &GPIO_InitStructure);
	GPIO_SetBits(OV7670_VSYNC_Port,OV7670_VSYNC_Pin);
		
 	GPIO_InitStructure.GPIO_Pin = OV7670_OE_Pin|OV7670_WEN_Pin|OV7670_WRST_Pin|OV7670_RRST_Pin|OV7670_RCLK_Pin;	 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                                                         //推挽输出
 	GPIO_Init(OV7670_OE_Port, &GPIO_InitStructure);
 	GPIO_SetBits(OV7670_OE_Port,OV7670_OE_Pin|OV7670_WEN_Pin|OV7670_WRST_Pin|OV7670_RRST_Pin|OV7670_RCLK_Pin);	

	GPIO_InitStructure.GPIO_Pin  = OV7670_DATA_Pin;                                                              //PA0~7 输入 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(OV7670_DATA_Port, &GPIO_InitStructure);

  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWD
}
/*********************************END OF FILE**********************************/
