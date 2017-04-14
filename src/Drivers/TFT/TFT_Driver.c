/*******************************************************************************
  * @file                   TFT_Driver.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.1.5
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "TFT_Driver.h"
#include "delay.h"
#include "stm32f10x_spi.h"
/* Definition ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  spi 写一个字节             
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
{
	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPIx->DR=Byte;	                            	//发送一个byte   
	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);   //等待接收完一个byte  
	return SPIx->DR;          	                  //返回收到的数据			
} 
/*******************************************************************************
  * @brief  设置SPI的速度           
  * @param  None              
  * @retval None              
  * @Note   SpeedSet:1,高速;0,低速;             
*******************************************************************************/
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet)
{
	SPIx->CR1&=0XFFC7;
	if(SpeedSet==1)                        //高速
	{
		SPIx->CR1|=SPI_BaudRatePrescaler_2;  //Fsck=Fpclk/2	
	}
	else                                   //低速
	{
		SPIx->CR1|=SPI_BaudRatePrescaler_32; //Fsck=Fpclk/32
	}
	SPIx->CR1|=1<<6;                       //SPI设备使能
} 
/*******************************************************************************
  * @brief  硬件SPI配置             
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void SPI2_Init(void)	
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);//配置SPI2管脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12;    //|GPIO_Pin_9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
	   
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    	//SPI2配置选项
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE);                                                  //使能SPI2
}
/*******************************************************************************
  * @brief  WriteIndex            
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_WriteIndex(u8 Index)
{
   TFT_RS_CLR;
   SPI_WriteByte(SPI2,Index);
}
/*******************************************************************************
  * @brief  WriteData       
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_WriteData(u8 Data)
{
   TFT_RS_SET;
   SPI_WriteByte(SPI2,Data);
}
/*******************************************************************************
  * @brief  WriteData16Bit 
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_WriteData16Bit(u8 DataH,u8 DataL)
{
	TFT_WriteData(DataH);
	TFT_WriteData(DataL);
}
/*******************************************************************************
  * @brief  WriteIndex16Bit 
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_WriteIndex16Bit(u8 DataH,u8 DataL)
{
	TFT_WriteIndex(DataH);
	TFT_WriteIndex(DataL);
}
/*******************************************************************************
  * @brief  TFT_Init
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_Init(void)
{
	SPI2_Init();
	TFT_RST_CLR;
	Delay_ms(100);
	TFT_RST_SET;
	Delay_ms(50);
	TFT_WriteIndex(0xCB);  
	TFT_WriteData(0x39); 
	TFT_WriteData(0x2C); 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x34); 
	TFT_WriteData(0x02); 
	TFT_WriteIndex(0xCF);  
	TFT_WriteData(0x00); 
	TFT_WriteData(0XC1); 
	TFT_WriteData(0X30); 
	TFT_WriteIndex(0xE8);  
	TFT_WriteData(0x85); 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x78); 
	TFT_WriteIndex(0xEA);  
	TFT_WriteData(0x00); 
	TFT_WriteData(0x00); 
	TFT_WriteIndex(0xED);  
	TFT_WriteData(0x64); 
	TFT_WriteData(0x03); 
	TFT_WriteData(0X12); 
	TFT_WriteData(0X81); 
	TFT_WriteIndex(0xF7);  
	TFT_WriteData(0x20); 
	TFT_WriteIndex(0xC0);    //Power control 
	TFT_WriteData(0x23);     //VRH[5:0] 
	TFT_WriteIndex(0xC1);    //Power control 
	TFT_WriteData(0x10);     //SAP[2:0];BT[3:0] 
	TFT_WriteIndex(0xC5);    //VCM control 
	TFT_WriteData(0x3e);     //对比度调节
	TFT_WriteData(0x28); 
	TFT_WriteIndex(0xC7);    //VCM control2 
	TFT_WriteData(0x86);   
	TFT_WriteIndex(0x36);    // Memory Access Control 
#ifdef H_VIEW 
	TFT_WriteData(0xE8);     //C8	   //48 68竖屏//28 E8 横屏
#else
	TFT_WriteData(0x48); 
#endif
	TFT_WriteIndex(0x3A);    
	TFT_WriteData(0x55); 
	TFT_WriteIndex(0xB1);    
	TFT_WriteData(0x00);  
	TFT_WriteData(0x18); 
	TFT_WriteIndex(0xB6);    // Display Function Control 
	TFT_WriteData(0x08); 
	TFT_WriteData(0x82);
	TFT_WriteData(0x27);  
	TFT_WriteIndex(0xF2);    // 3Gamma Function Disable 
	TFT_WriteData(0x00); 
	TFT_WriteIndex(0x26);    //Gamma curve selected 
	TFT_WriteData(0x01); 
	TFT_WriteIndex(0xE0);    //Set Gamma 
	TFT_WriteData(0x0F); 
	TFT_WriteData(0x31); 
	TFT_WriteData(0x2B); 
	TFT_WriteData(0x0C); 
	TFT_WriteData(0x0E); 
	TFT_WriteData(0x08); 
	TFT_WriteData(0x4E); 
	TFT_WriteData(0xF1); 
	TFT_WriteData(0x37); 
	TFT_WriteData(0x07); 
	TFT_WriteData(0x10); 
	TFT_WriteData(0x03); 
	TFT_WriteData(0x0E); 
	TFT_WriteData(0x09); 
	TFT_WriteData(0x00); 
	TFT_WriteIndex(0XE1);    //Set Gamma 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x0E); 
	TFT_WriteData(0x14); 
	TFT_WriteData(0x03); 
	TFT_WriteData(0x11); 
	TFT_WriteData(0x07); 
	TFT_WriteData(0x31); 
	TFT_WriteData(0xC1); 
	TFT_WriteData(0x48); 
	TFT_WriteData(0x08); 
	TFT_WriteData(0x0F); 
	TFT_WriteData(0x0C); 
	TFT_WriteData(0x31); 
	TFT_WriteData(0x36); 
	TFT_WriteData(0x0F); 
	TFT_WriteIndex(0x11);    //Exit Sleep 
	Delay_ms(120); 
	TFT_WriteIndex(0x29);    //Display on 
	TFT_WriteIndex(0x2c); 
}

/*******************************************************************************
  * @brief  TFT_Set_Region
  * @param  None              
  * @retval None              
  * @Note   设置lcd显示区域，在此区域写点数据自动换行
            入口参数：xy起点和终点,Y_IncMode表示先自增y再自增x         
*******************************************************************************/
void TFT_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{	
	TFT_WriteIndex(0x2a);
	TFT_WriteData16Bit(x_start>>8,x_start);
	TFT_WriteData16Bit(x_end>>8,x_end);
	TFT_WriteIndex(0x2b);
	TFT_WriteData16Bit(y_start>>8,y_start);
	TFT_WriteData16Bit(y_end>>8,y_end);
	TFT_WriteIndex(0x2c);

}
/*******************************************************************************
  * @brief  TFT_Set_XY
  * @param  xy坐标           
  * @retval None              
  * @Note   设置lcd显示起始点   
*******************************************************************************/
void TFT_SetXY(u16 x,u16 y)
{
	TFT_WriteIndex(0x2a);
	TFT_WriteData16Bit(x>>8,x);
	TFT_WriteIndex(0x2b);
	TFT_WriteData16Bit(y>>8,y);
	TFT_WriteIndex(0x2c);
}
/*******************************************************************************
  * @brief  TFT_DrawPoint
  * @param  xy坐标           
  * @retval None              
  * @Note   画一个点
*******************************************************************************/
void TFT_DrawPoint(u16 x,u16 y,u16 Data)
{
	TFT_SetXY(x,y);
	TFT_WriteData(Data>>8);
	TFT_WriteData(Data);
}    
/*******************************************************************************
  * @brief  读TFT某一点的颜色    
  * @param  None           
  * @retval None              
  * @Note   None 
*******************************************************************************/
u16 TFT_ReadPoint(u16 x,u16 y)
{
	u8 r,g,b;
	u16 R,G,B,Data;
	TFT_SetXY(x,y);  
	TFT_CS_CLR;
	TFT_WriteIndex(0X2E);         //第一次写入控制命令
	TFT_RS_SET;
	TFT_RS_CLR;                   //产生脉冲
	SPI_WriteByte(SPI2,0xff);     //第二次空读写DUMMY CLOCK
	r=SPI_WriteByte(SPI2,0xff);   //高六位有效
	g=SPI_WriteByte(SPI2,0xff);   //高六位有效
	b=SPI_WriteByte(SPI2,0xff);   //高六位有效
	TFT_CS_SET;
	R = (r<<1)&0x00FF;
	G = g&0x00FF;
	B = (b<<1)&0x00FF;
	Data = 	(R<<8)|(G<<5)|(B>>3);
	return Data;
}

/*
		r=LCD->LCD_RAM;  		  						
		delay_us(2);	  
		b=LCD->LCD_RAM; 
		g=r&0XFF;//9341要分两次读出
		g<<=8;
		return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
*/

/*******************************************************************************
  * @brief  全屏清屏函数
  * @param  None           
  * @retval None              
  * @Note   None 
*******************************************************************************/
void TFT_Clear(u16 Color)               
{	
	unsigned int i,m;
	TFT_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
	TFT_RS_SET;
   
	for(i=0;i<Y_MAX_PIXEL;i++)
	{
		for(m=0;m<X_MAX_PIXEL;m++)
		{	 
			SPI_WriteByte(SPI2,Color>>8);
			SPI_WriteByte(SPI2,Color);
		}   
	}
}
/*********************************END OF FILE**********************************/

