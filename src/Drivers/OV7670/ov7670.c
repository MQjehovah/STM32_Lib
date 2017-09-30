/*******************************************************************************
  * @file                   ov7670.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.2.28
  * @brief                  ������ο���ALIENTEKս��STM32������V3�������
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
  * @brief  ��ʼ��OV7670              
  * @param  None              
  * @retval None              
  * @Note   ����0:�ɹ�  
            ��������ֵ:�������	
*******************************************************************************/
u8 OV7670_Init(void)
{
	u8 temp;
	u16 i=0;	

//	OV7670_GPIO_Init();                      //��ʼ��IO
 	SCCB_Init();        		                 //��ʼ��SCCB ��IO��	 
	
#ifdef debug
	printf("SCCB��ʼ���ɹ�\n");
#endif
	
 	if(SCCB_WR_Reg(0x12,0x80))return 1;	     //��λSCCB	 
	
#ifdef debug
	printf("��λSCCB�ɹ�\n");
#endif	
	
	Delay_ms(50);  	
 	temp=SCCB_RD_Reg(0x0b);                  //��ȡ��Ʒ�ͺ�
	
#ifdef debug
	printf("��ȡ�ɹ�\n");
	printf("temp=%d\n",temp);
#endif
	
	if(temp!=0x73)return 2;  
 	temp=SCCB_RD_Reg(0x0a);   
	if(temp!=0x76)return 2;
	for(i=0;i<sizeof(ov7670_init_reg_tbl)/sizeof(ov7670_init_reg_tbl[0]);i++)  //��ʼ������	  
	{
	   	SCCB_WR_Reg(ov7670_init_reg_tbl[i][0],ov7670_init_reg_tbl[i][1]);
  	}
   	return 0x00; 	                         //ok
} 
/*******************************************************************************
  * @brief  ���ô���           
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
	temp=SCCB_RD_Reg(0X03);				        //��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				        //����Vref��start��end�����2λ
	SCCB_WR_Reg(0X19,sx>>2);			        //����Vref��start��8λ
	SCCB_WR_Reg(0X1A,endx>>2);		       	//����Vref��end�ĸ�8λ

	temp=SCCB_RD_Reg(0X32);			        	//��ȡHref֮ǰ��ֵ
	temp&=0XC0; 
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X17,sy>>3);			        //����Href��start��8λ
	SCCB_WR_Reg(0X18,endy>>3);			      //����Href��end�ĸ�8λ
}
/*******************************************************************************
  * @brief  GPIO��ʼ��           
  * @param  None              
  * @retval None              
  * @Note   None	
*******************************************************************************/
void OV7670_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ����ض˿�ʱ��
 
	GPIO_InitStructure.GPIO_Pin = OV7670_VSYNC_Pin; 	                                               //VSYNC ���� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(OV7670_VSYNC_Port, &GPIO_InitStructure);
	GPIO_SetBits(OV7670_VSYNC_Port,OV7670_VSYNC_Pin);
		
 	GPIO_InitStructure.GPIO_Pin = OV7670_OE_Pin|OV7670_WEN_Pin|OV7670_WRST_Pin|OV7670_RRST_Pin|OV7670_RCLK_Pin;	 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                                                         //�������
 	GPIO_Init(OV7670_OE_Port, &GPIO_InitStructure);
 	GPIO_SetBits(OV7670_OE_Port,OV7670_OE_Pin|OV7670_WEN_Pin|OV7670_WRST_Pin|OV7670_RRST_Pin|OV7670_RCLK_Pin);	

	GPIO_InitStructure.GPIO_Pin  = OV7670_DATA_Pin;                                                              //PA0~7 ���� ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(OV7670_DATA_Port, &GPIO_InitStructure);

  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWD
}
/*********************************END OF FILE**********************************/
