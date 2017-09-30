/*******************************************************************************
  * @file                   sccb.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.1.5
  * @brief                  ������ο���ALIENTEKս��STM32������V3�������
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "sccb.h"
#include "USART_Driver.h"
#include "delay.h"
/* Definition ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  ��ʼ��SCCB�ӿ�              
  * @param  None              
  * @retval None              
  * @Note   CHECK OK             
*******************************************************************************/
void SCCB_Init(void)
{			
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	            //ʹ��PB�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = SCCB_SIC_Pin|SCCB_SID_Pin;				  //�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		              //����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	SCCB_SIC_H;						                                            //�����
 	SCCB_SID_H;
	//SCCB_SDA_OUT;	   
}			 
/*******************************************************************************
  * @brief  SCCB��ʼ�ź�             
  * @param  None              
  * @retval None              
  * @Note   ��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�  
            �ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
*******************************************************************************/
void SCCB_Start(void)
{
    SCCB_SID_H;     //�����߸ߵ�ƽ	   
    SCCB_SIC_H;     //��ʱ���߸ߵ�ʱ���������ɸ�����
    Delay_us(50);  
    SCCB_SID_L;
    Delay_us(50);	 
    SCCB_SIC_L;	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}
/*******************************************************************************
  * @brief  SCCBֹͣ�ź�           
  * @param  None              
  * @retval None              
  * @Note   ��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
            ����״����,SDA,SCL��Ϊ�ߵ�ƽ
*******************************************************************************/
void SCCB_Stop(void)
{
    SCCB_SID_L;
    Delay_us(50);	 
    SCCB_SIC_H;	
    Delay_us(50); 
    SCCB_SID_H;	
    Delay_us(50);
}  

/*******************************************************************************
  * @brief  ����NA�ź�          
  * @param  None              
  * @retval None              
  * @Note   None 
*******************************************************************************/
void SCCB_No_Ack(void)
{
	Delay_us(50);
	SCCB_SID_H;	
	SCCB_SIC_H;	
	Delay_us(50);
	SCCB_SIC_L;	
	Delay_us(50);
	SCCB_SID_L;	
	Delay_us(50);
}
/*******************************************************************************
  * @brief  д��һ���ֽ�         
  * @param  None              
  * @retval None              
  * @Note   ����ֵ:0,�ɹ�;1,ʧ��. 
*******************************************************************************/
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++)               //ѭ��8�η�������
	{
		if(dat&0x80)SCCB_SID_H;
		else SCCB_SID_L;
		dat<<=1;
		Delay_us(50);
		SCCB_SIC_H;	
		Delay_us(50);
		SCCB_SIC_L;		   
	}			 
	SCCB_SDA_IN;		               //����SDAΪ���� 
	Delay_us(50);
	SCCB_SIC_H;			               //���յھ�λ,���ж��Ƿ��ͳɹ�
	Delay_us(50);
	if(SCCB_READ_SID)res=1;        //SDA=1����ʧ�ܣ�����1
	else res=0;                    //SDA=0���ͳɹ�������0
	SCCB_SIC_L;		 
	SCCB_SDA_OUT;	                 //����SDAΪ���    
	return res;  
}	 
/*******************************************************************************
  * @brief  ��ȡһ���ֽ�    
  * @param  None              
  * @retval None              
  * @Note   ��SCL��������,��������
						����ֵ:����������
*******************************************************************************/
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN;		  //����SDAΪ���� 
	
#ifdef debug
	printf("����SCCB���ֽں���\n");
#endif
	
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
	{		     	  
		Delay_us(50);
		SCCB_SIC_H;
		temp=temp<<1;
		if(SCCB_READ_SID)temp++;
		Delay_us(50);
		SCCB_SIC_L;
	}	
	SCCB_SDA_OUT;		  //����SDAΪ��� 
#ifdef debug
	if(temp==0)printf("û�ж�������\n");
#endif	 
	return temp;
} 	
/*******************************************************************************
  * @brief  д�Ĵ���    
  * @param  None              
  * @retval None              
  * @Note   ����ֵ:0,�ɹ�;1,ʧ��.
*******************************************************************************/
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					        //����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//д����ID	  
	Delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	Delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//д����	 
  	SCCB_Stop();	  
  	return	res;
}	
/*******************************************************************************
  * @brief  ���Ĵ���   
  * @param  None              
  * @retval None              
  * @Note   ����ֵ:�����ļĴ���ֵ
*******************************************************************************/
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				         //����SCCB����
	SCCB_WR_Byte(SCCB_ID);		     //д����ID	  
	Delay_us(100);	 
  SCCB_WR_Byte(reg);			       //д�Ĵ�����ַ	  
	Delay_us(100);	  
	SCCB_Stop();   
	Delay_us(100);	   
	SCCB_Start();                  //���üĴ�����ַ�󣬲��Ƕ�
	SCCB_WR_Byte(SCCB_ID|0X01);	   //���Ͷ�����	  
	Delay_us(100);
  val=SCCB_RD_Byte();		    	   //��ȡ����
  SCCB_No_Ack();
  SCCB_Stop();
  return val;
}

void SCCB_SID_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   /* Enable GPIOA  clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = SCCB_SID_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SCCB_SID_Port, &GPIO_InitStructure);
}
void SCCB_SID_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

   /* Enable GPIOA  clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = SCCB_SID_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SCCB_SID_Port,&GPIO_InitStructure);
}
/*********************************END OF FILE**********************************/
