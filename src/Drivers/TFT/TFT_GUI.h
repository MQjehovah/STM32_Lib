#include "stm32f10x.h"
int GetGBKCode_from_sd(unsigned char* pBuffer,unsigned char * c);

void LCD_Char_CH(u16 x,u16 y,unsigned char *str,u16 Color,u16 bkColor) ;
void LCD_Str_CH(u16 x,u16 y,unsigned char *str,u16 Color,u16 bkColor) ;

void TFT_Circle(u16 X,u16 Y,u16 R,u16 fc); 
void TFT_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color);  
void TFT_box(u16 x, u16 y, u16 w, u16 h,u16 bc);
void TFT_box2(u16 x,u16 y,u16 w,u16 h, u8 mode);
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2);
void TFT_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void TFT_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
void TFT_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num) ;
