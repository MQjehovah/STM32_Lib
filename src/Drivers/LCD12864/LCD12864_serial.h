/*******************************************************************************
  * @file                   LCD12864_serial.h	
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.2.28
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
#ifndef __LCD12864_SERIAL_H
#define __LCD12864_SERIAL_H

/* Includes ------------------------------------------------------------------*/

/* Definition ----------------------------------------------------------------*/
//#define RS_GPIO   GPIOA
#define RW_GPIO   GPIOA
#define EN_GPIO   GPIOA
//#define RS_CS  GPIO_Pin_10
#define RW_SID GPIO_Pin_11
#define EN_CLK GPIO_Pin_12

extern unsigned char const* pfont;
extern unsigned char PicBuff[16*64];

/* Exported Functions --------------------------------------------------------*/
void LCD12864_Init(void);
void LCD12864_Clear(void);
void LCD12864_ClearGRAM(void);
void LCD12864_WriteDat(unsigned char data);
void LCD12864_DrawPicture(const unsigned char *ptr);
void LCD12864_SetLocation(unsigned char x,unsigned char y);
void LCD12864_WriteStr(unsigned char dis_addr_x,unsigned char dis_addr_y,char* str);
/*以下函数使用12864缓存PicBuff画图*/
void LCD12864_ClearPicBuff(void);
void LCD12864_DisPlayPicBuff(void);
void LCD12864_DrawCircle(unsigned char x0,unsigned char y0,unsigned char r);
void LCD12864_Line_X(unsigned char X0,unsigned char X1,unsigned char Y,unsigned char Color);
void LCD12864_Line_Y(unsigned char X,unsigned char Y0,unsigned char Y1,unsigned char Color);
void LCD12864_DrawPoint(unsigned char *Buff, unsigned char x, unsigned char y, unsigned char color);
void LCD12864_Line(unsigned char X0,unsigned char Y0,unsigned char X1,unsigned char Y1,unsigned char Color);

#endif
/*********************************END OF FILE**********************************/
