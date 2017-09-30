/*******************************************************************************
  * @file                   LCD12864_serial.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.1.5
  * @brief                  LCD12864串行驱动，字符、汉字显示、图形显示
  ******************************************************************************
  * @attention              需调用Delay_us()、Delay_ms()
	                          LCD12864  PSB(15脚)需接低电平
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Delay.h"
#include "stm32f10x.h"
#include "LCD12864_serial.h"
#include "math.h"
/* Definition ----------------------------------------------------------------*/
#define SENDCMD  0XF8     //0XF1000 
#define READCMD  0XFC     //0XF1100
#define SENDDATA 0XFA     //0XF1010
#define READDATA 0XFE     //0XF1110

unsigned char PicBuff[16*64];
/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  传送1Byte数据到LCD12864               
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/      
void LCD12864_SendByte(unsigned char data)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(data&0x80)
			GPIO_SetBits(RW_GPIO, RW_SID);
		else
			GPIO_ResetBits(RW_GPIO, RW_SID);
    Delay_us(1);
		GPIO_SetBits(EN_GPIO, EN_CLK);
    data <<= 1;
    Delay_us(1);
    GPIO_ResetBits(EN_GPIO, EN_CLK);
	}
}
/*******************************************************************************
  * @brief  向LCD12864写1Byte命令               
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void LCD12864_WriteCmd(unsigned char cmd)
{
	unsigned char temp;
//  GPIO_SetBits(RS_GPIO, RS_CS);
	LCD12864_SendByte(SENDCMD);
	
	temp=cmd&0xF0;
	LCD12864_SendByte(temp);
		
	temp=((cmd&0x0F)<<4)&0xF0;
	LCD12864_SendByte(temp);
//  GPIO_ResetBits(RS_GPIO, RS_CS);
}
/*******************************************************************************
  * @brief  向LCD12864写入1Byte数据               
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void LCD12864_WriteDat(unsigned char data)
{
	unsigned char temp;
//GPIO_SetBits(RS_GPIO, RS_CS);
	LCD12864_SendByte(SENDDATA);
	
	temp=data&0xf0;
	LCD12864_SendByte(temp);
	
	temp=(data&0X0F)<<4;
	LCD12864_SendByte(temp);
//GPIO_ResetBits(RS_GPIO, RS_CS);
}
/*******************************************************************************
  * @brief  设置LCD12864显示位置             
  * @param  None              
  * @retval None              
  * @Note   x(1 ~ 16) 
						y(1 ~  4)
*******************************************************************************/
void LCD12864_SetLocation(unsigned char x,unsigned char y)
{
	switch(y)
	{
		case 0x01:
			LCD12864_WriteCmd(0X7F+x);break;
		case 0x02:
			LCD12864_WriteCmd(0X8F+x);break;
		case 0x03:
			LCD12864_WriteCmd(0X87+x);break;
		case 0x04:
			LCD12864_WriteCmd(0X97+x);break;
		default:break;
	}
}
/*******************************************************************************
  * @brief  在指定位置显示字符串                 
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void LCD12864_WriteStr(unsigned char dis_addr_x,unsigned char dis_addr_y,char* str)
{
    LCD12864_SetLocation(dis_addr_x,dis_addr_y);
    while(*str != 0x00) 
    {
        LCD12864_WriteDat(*str++);
    }
}
/*******************************************************************************
  * @brief  LCD12864初始化                
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void LCD12864_init(void)
{
  GPIO_ResetBits(EN_GPIO, EN_CLK);
	Delay_us(1);
	Delay_us(500);
	LCD12864_WriteCmd(0x30);		//功能设置，一次送8位数据，基本指令集
	LCD12864_WriteCmd(0x0C);		//0000,1100 整体显示，游标off，游标位置off
	LCD12864_WriteCmd(0x01);		//0000,0001 清DDRAM
	LCD12864_WriteCmd(0x02);		//0000,0010 DDRAM地址归位
//	LCD12864_WriteCmd(0x80);		//1000,0000 设定DDRAM 7位地址000，0000到地址计数器AC
//	LCD12864_WriteCmd(0x04);		//点设定，显示字符/光标从左到右移位，DDRAM地址加 一
//	LCD12864_WriteCmd(0x0F);		//显示设定，开显示，显示光标，当前显示位反白闪动
  LCD12864_ClearGRAM();
}
/*******************************************************************************
  * @brief  GPIO初始化                
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void LCD12864_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure = {RW_SID, GPIO_Speed_50MHz, GPIO_Mode_Out_PP};
	GPIO_Init(RW_GPIO, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = RS_CS;
//  GPIO_Init(RS_GPIO, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = EN_CLK;
  GPIO_Init(EN_GPIO, &GPIO_InitStructure);
	LCD12864_init();
}
/*******************************************************************************
  * @brief  清除LCD12864显示                
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void LCD12864_Clear(void)
{
	LCD12864_WriteCmd(0x01);
	Delay_ms(10);
}
/*******************************************************************************
  * @brief  LCD12864显示128*64图片               
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void LCD12864_DrawPicture(const unsigned char *ptr)
{
    unsigned int i,j,k;
	LCD12864_WriteCmd(0x34);       //打开扩展指令集
	for(i=0x80,j=0;j<32;j++)
	{
        LCD12864_WriteCmd(i++);
        LCD12864_WriteCmd(0x80);
  		for(k=0;k<16;k++) 
			LCD12864_WriteDat(*ptr++);
	}

 	for(i=0x80,j=0;j<32;j++)
	{
 	    LCD12864_WriteCmd(i++);
        LCD12864_WriteCmd(0x88);
  		for(k=0;k<16;k++)     
			LCD12864_WriteDat(*ptr++);
	}
    LCD12864_WriteCmd(0x36);         //打开绘图显示
	LCD12864_WriteCmd(0x30);        //回到基本指令集
}
void LCD12864_ClearGRAM(void)
{
	unsigned char i,j,k;
	LCD12864_WriteCmd(0x01);
	Delay_ms(10);
	LCD12864_WriteCmd(0x34); 
	for(i=0x80,j=0;j<32;j++){
        LCD12864_WriteCmd(i++);
        LCD12864_WriteCmd(0x80);
  		for(k=0;k<16;k++) 
			LCD12864_WriteDat(0x00);
	}
 	for(i=0x80,j=0;j<32;j++){
 	    LCD12864_WriteCmd(i++);
        LCD12864_WriteCmd(0x88);
  		for(k=0;k<16;k++)     
			LCD12864_WriteDat(0x00);
	}
	LCD12864_WriteCmd(0x30);
}



void LCD12864_ClearPicBuff(void)
{
  unsigned int i;
  for(i=0; i<1024; i++)
    PicBuff[i]=0x00;
}
void LCD12864_DisPlayPicBuff(void)
{
  LCD12864_DrawPicture(PicBuff);
}

void LCD12864_DrawPoint(unsigned char *Buff, unsigned char x, unsigned char y, unsigned char color)
{
  unsigned char pic_bit;
	unsigned int pic_byte;
  x&=0x7F;
  y&=0x3F;
	pic_bit=x&0x07;
	pic_byte=(y<<4)|(x>>3);
  switch(color)
  {
    case 0x00:Buff[pic_byte]&=~(0x80>>pic_bit);break;//12864串行先写高位，故此处高低位交换
    case 0x01:Buff[pic_byte]|=0x80>>pic_bit;break;
    case 0x02:Buff[pic_byte]^=0x80>>pic_bit;break;
    default :break;
  }
}

void LCD12864_Line_X(unsigned char X0,unsigned char X1,unsigned char Y,unsigned char Color)
{
	unsigned char temp;
	if (X0>X1){
	   temp=X1;
	   X1=X0;
	   X0=temp;
	}
	for(;X0<X1;X0++)
	   LCD12864_DrawPoint(PicBuff,X0,Y,Color);
}

void LCD12864_Line_Y(unsigned char X,unsigned char Y0,unsigned char Y1,unsigned char Color)
{
	unsigned char temp;
	if (Y0>Y1){
	   temp=Y1;
	   Y1=Y0;
	   Y0=temp;
	}
	for(;Y0<Y1;Y0++)
	   LCD12864_DrawPoint(PicBuff,X,Y0,Color);
}
void LCD12864_Line(unsigned char X0,unsigned char Y0,unsigned char X1,unsigned char Y1,unsigned char Color)
{
    int t,distance;
    int x=0,y=0,delta_x,delta_y;
    int incx,incy;
    delta_x=X1-X0;
    delta_y=Y1-Y0;
    if(delta_x>0)
        incx=1;
    else if(delta_x==0){
        LCD12864_Line_Y(X0,Y0,Y1,Color);
        return ;
    }
    else
        incx=-1;
    if(delta_y>0)
        incy=1;
    else if(delta_y==0){
        LCD12864_Line_X(X0,X1,Y0,Color);    
        return ;
    }
    else
        incy=-1;
    delta_x=fabs(delta_x);    
    delta_y=fabs(delta_y);
    if(delta_x>delta_y)
        distance=delta_x;
    else
        distance = delta_y; 
    LCD12864_DrawPoint(PicBuff,X0,Y0,Color);
    for(t=0;t<=distance+1;t++)
    {
        LCD12864_DrawPoint(PicBuff,X0,Y0,Color);
        x+=delta_x;
        y+=delta_y;
        if(x>distance){
            x-=distance;
            X0+=incx;
        }
        if(y>distance ){
            y-=distance;
            Y0+=incy;
        }
    }
}
void LCD12864_DrawCircle(unsigned char x0,unsigned char y0,unsigned char r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下一个点的位置的标志
	while(a<=b)
	{
		LCD12864_DrawPoint(PicBuff,x0-b,y0-a,1);             //3           
		LCD12864_DrawPoint(PicBuff,x0+b,y0-a,1);             //0           
		LCD12864_DrawPoint(PicBuff,x0-a,y0+b,1);             //1       
		LCD12864_DrawPoint(PicBuff,x0-b,y0-a,1);             //7           
		LCD12864_DrawPoint(PicBuff,x0-a,y0-b,1);             //2             
		LCD12864_DrawPoint(PicBuff,x0+b,y0+a,1);             //4               
		LCD12864_DrawPoint(PicBuff,x0+a,y0-b,1);             //5
		LCD12864_DrawPoint(PicBuff,x0+a,y0+b,1);             //6 
		LCD12864_DrawPoint(PicBuff,x0-b,y0+a,1);             
		a++;
		//使用Bresenham算法画圆    
		if(di<0)di+=4*a+6;	  
		else{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD12864_DrawPoint(PicBuff,x0+a,y0+b,1);
	}
}
/*
typedef struct
{
  unsigned char Width;
  unsigned char Height;
  unsigned int len;
  const void *Font;
}sFont;
// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct                  // 汉字字模数据结构
{
       signed char Index[2];               // 汉字内码索引
       char Msk[32];                       // 点阵码数据
}typFNT_GB16;
/////////////////////////////////////////////////////////////////////////
// 汉字字模表                                                          //
// 汉字库: 宋体16.dot,横向取模左高位,数据排列:从左到右从上到下         //
/////////////////////////////////////////////////////////////////////////
typFNT_GB16 const GB_16[] =          // 数据表
{
"我", 0x04,0x80,0x0E,0xA0,0x78,0x90,0x08,0x90,
      0x08,0x84,0xFF,0xFE,0x08,0x80,0x08,0x90,
      0x0A,0x90,0x0C,0x60,0x18,0x40,0x68,0xA0,
      0x09,0x20,0x0A,0x14,0x28,0x14,0x10,0x0C,

"是", 0x0F,0xE0,0x08,0x20,0x08,0x20,0x0F,0xE0,
      0x08,0x20,0x08,0x20,0x0F,0xE0,0x00,0x04,
      0xFF,0xFE,0x01,0x00,0x09,0x20,0x09,0xF0,
      0x09,0x00,0x15,0x00,0x23,0x06,0x40,0xFC,

"特", 0x10,0x40,0x10,0x40,0x50,0x48,0x53,0xFC,
      0x7C,0x40,0x90,0x44,0x17,0xFE,0x18,0x10,
      0x30,0x14,0xD7,0xFE,0x11,0x10,0x10,0x90,
      0x10,0x90,0x10,0x10,0x10,0x50,0x10,0x20,

"种", 0x08,0x20,0x1C,0x20,0xF0,0x20,0x10,0x24,
      0x11,0xFE,0xFF,0x24,0x11,0x24,0x39,0x24,
      0x35,0x24,0x55,0xFC,0x51,0x24,0x90,0x20,
      0x10,0x20,0x10,0x20,0x10,0x20,0x10,0x20,

"兵", 0x00,0x20,0x00,0xF0,0x0F,0x00,0x08,0x00,
      0x08,0x00,0x08,0x10,0x0F,0xF8,0x08,0x40,
      0x08,0x40,0x08,0x44,0xFF,0xFE,0x00,0x00,
      0x04,0x40,0x0C,0x30,0x10,0x18,0x20,0x08
};
// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct                  // 汉字字模数据结构
{
       signed char Index[2];               // 汉字内码索引
       char Msk[24];                       // 点阵码数据
}typFNT_GB12;

/////////////////////////////////////////////////////////////////////////
// 汉字字模表                                                          //
// 汉字库: 宋体12.dot,横向取模左高位,数据排列:从左到右从上到下         //
/////////////////////////////////////////////////////////////////////////
typFNT_GB12 const GB_12[] =          // 数据表
{
"芜", 0x09,0x20,0xFF,0xF0,0x09,0x00,0x3F,0xC0,
      0x04,0x00,0x04,0x00,0xFF,0xF0,0x05,0x00,
      0x09,0x00,0x11,0x10,0x21,0x10,0xC0,0xF0,

"湖", 0x44,0x00,0x24,0xF0,0x0E,0x90,0x84,0x90,
      0x44,0xF0,0x2E,0x90,0x4A,0x90,0xCA,0xF0,
      0x4E,0x90,0x48,0x90,0x41,0x50,0x02,0x30,
};

sFont Font_GB16 ={16,16,5,(typFNT_GB16*)GB_16};
sFont Font_GB12 ={12,12,2,(typFNT_GB12*)GB_12};
sFont *pFontTable[]=
{
  &Font_GB16,
};
unsigned char const* pfont=(void*)0;
void sousuo(sFont *font, unsigned char *pData)
{
  unsigned int index;
  unsigned int fontsize;
  pfont = (unsigned char*)font->Font;
//  fontsize = ((font->Height)*(font->Width))>>3;
  if(font==&Font_GB12)
    fontsize = 24;
  else if(font==&Font_GB16)
    fontsize = 32;
  for(index=0; index<font->len; index++)
  {
    if(*pfont==*pData&&*(pfont+1)==*(pData+1))
    {
      pfont+=2;
      return;
    }
    pfont+=fontsize+2;
  }
  pfont=(void*)0;
}
void LCD12864_WriteStrBuff(sFont *font, char x, char y, unsigned char *pData, char len)
{
  unsigned char i,j;
  unsigned char pic_bit;
	unsigned int pic_byte;
  for(i=0; i<len; i++)
  {
    sousuo(font,pData);
    if(pfont==(void*)0)
      return;
    pData+=2;
    for(j=0; j<font->Height; j++)
    {
      pic_byte = ((y+j)<<4|(x+i*font->Width)>>3);
      pic_bit = (x+i*font->Width)&0x07;
      PicBuff[pic_byte]&=(unsigned char)(0xff<<(8-pic_bit));
      PicBuff[pic_byte]|=(unsigned char)(*pfont>>pic_bit);
      PicBuff[pic_byte+1]&=(unsigned char)(0xff>>pic_bit);
      PicBuff[pic_byte+1]|=(unsigned char)(*pfont++<<(8-pic_bit));
      if(font==&Font_GB16)
      {
        PicBuff[pic_byte+1]&=(unsigned char)(0xff<<(8-pic_bit));
        PicBuff[pic_byte+1]|=(unsigned char)(pic_bit==0?*pfont++:*pfont>>pic_bit);
      }
      else if(font==&Font_GB12)
      {
        PicBuff[pic_byte+1]&=(unsigned char)((0xff<<(8-pic_bit))|(0xff>>(4-pic_bit)));
        PicBuff[pic_byte+1]|=(unsigned char)(pic_bit<=5?*pfont++>>pic_bit:*pfont>>pic_bit);
      }
      if(pic_bit!=0)
      {
        PicBuff[pic_byte+2]&=(unsigned char)(0xff>>pic_bit);
        PicBuff[pic_byte+2]|=(unsigned char)(*pfont++<<(font->Width==12?5:8-pic_bit));
      }
//      LCD12864_DisPlayPicBuff();
    }
  }
}
*/
/*********************************END OF FILE**********************************/
