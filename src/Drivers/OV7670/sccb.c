/*******************************************************************************
  * @file                   sccb.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.1.5
  * @brief                  本程序参考了ALIENTEK战舰STM32开发板V3相关资料
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
  * @brief  初始化SCCB接口              
  * @param  None              
  * @retval None              
  * @Note   CHECK OK             
*******************************************************************************/
void SCCB_Init(void)
{			
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	            //使能PB端口时钟
	
  GPIO_InitStructure.GPIO_Pin = SCCB_SIC_Pin|SCCB_SID_Pin;				  //端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		              //输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	SCCB_SIC_H;						                                            //输出高
 	SCCB_SID_H;
	//SCCB_SDA_OUT;	   
}			 
/*******************************************************************************
  * @brief  SCCB起始信号             
  * @param  None              
  * @retval None              
  * @Note   当时钟为高的时候,数据线的高到低,为SCCB起始信号  
            在激活状态下,SDA和SCL均为低电平
*******************************************************************************/
void SCCB_Start(void)
{
    SCCB_SID_H;     //数据线高电平	   
    SCCB_SIC_H;     //在时钟线高的时候数据线由高至低
    Delay_us(50);  
    SCCB_SID_L;
    Delay_us(50);	 
    SCCB_SIC_L;	    //数据线恢复低电平，单操作函数必要	  
}
/*******************************************************************************
  * @brief  SCCB停止信号           
  * @param  None              
  * @retval None              
  * @Note   当时钟为高的时候,数据线的低到高,为SCCB停止信号
            空闲状况下,SDA,SCL均为高电平
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
  * @brief  产生NA信号          
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
  * @brief  写入一个字节         
  * @param  None              
  * @retval None              
  * @Note   返回值:0,成功;1,失败. 
*******************************************************************************/
u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++)               //循环8次发送数据
	{
		if(dat&0x80)SCCB_SID_H;
		else SCCB_SID_L;
		dat<<=1;
		Delay_us(50);
		SCCB_SIC_H;	
		Delay_us(50);
		SCCB_SIC_L;		   
	}			 
	SCCB_SDA_IN;		               //设置SDA为输入 
	Delay_us(50);
	SCCB_SIC_H;			               //接收第九位,以判断是否发送成功
	Delay_us(50);
	if(SCCB_READ_SID)res=1;        //SDA=1发送失败，返回1
	else res=0;                    //SDA=0发送成功，返回0
	SCCB_SIC_L;		 
	SCCB_SDA_OUT;	                 //设置SDA为输出    
	return res;  
}	 
/*******************************************************************************
  * @brief  读取一个字节    
  * @param  None              
  * @retval None              
  * @Note   在SCL的上升沿,数据锁存
						返回值:读到的数据
*******************************************************************************/
u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN;		  //设置SDA为输入 
	
#ifdef debug
	printf("进入SCCB读字节函数\n");
#endif
	
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		Delay_us(50);
		SCCB_SIC_H;
		temp=temp<<1;
		if(SCCB_READ_SID)temp++;
		Delay_us(50);
		SCCB_SIC_L;
	}	
	SCCB_SDA_OUT;		  //设置SDA为输出 
#ifdef debug
	if(temp==0)printf("没有读到数据\n");
#endif	 
	return temp;
} 	
/*******************************************************************************
  * @brief  写寄存器    
  * @param  None              
  * @retval None              
  * @Note   返回值:0,成功;1,失败.
*******************************************************************************/
u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					        //启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	Delay_us(100);
  	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	Delay_us(100);
  	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
  	SCCB_Stop();	  
  	return	res;
}	
/*******************************************************************************
  * @brief  读寄存器   
  * @param  None              
  * @retval None              
  * @Note   返回值:读到的寄存器值
*******************************************************************************/
u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				         //启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		     //写器件ID	  
	Delay_us(100);	 
  SCCB_WR_Byte(reg);			       //写寄存器地址	  
	Delay_us(100);	  
	SCCB_Stop();   
	Delay_us(100);	   
	SCCB_Start();                  //设置寄存器地址后，才是读
	SCCB_WR_Byte(SCCB_ID|0X01);	   //发送读命令	  
	Delay_us(100);
  val=SCCB_RD_Byte();		    	   //读取数据
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
