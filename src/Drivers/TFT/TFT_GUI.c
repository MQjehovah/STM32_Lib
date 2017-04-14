/*******************************************************************************
  * @file                   TFT_GUI.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.1.5
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
//#include "ff.h"
#include "TFT_GUI.h"
#include "TFT_Driver.h"
#include "stdio.h"
#include "font.h"

/* Definition ----------------------------------------------------------------*/
//#define SD_FONT
#ifdef SD_FONT
extern   FATFS fs;         //Work area (file system object) for logical drive 
extern   FIL  fdst;        // file objects 
extern  FRESULT res;
extern   UINT br;                  
#endif

/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  画圆              
  * @param  None              
  * @retval None              
  * @Note   Bresenham算法             
*******************************************************************************/
void TFT_Circle(u16 X,u16 Y,u16 R,u16 fc)  
{
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        TFT_DrawPoint(X+a,Y+b,fc);     //        7 
        TFT_DrawPoint(X-a,Y+b,fc);     //        6 
        TFT_DrawPoint(X+a,Y-b,fc);     //        2 
        TFT_DrawPoint(X-a,Y-b,fc);     //        3 
        TFT_DrawPoint(X+b,Y+a,fc);     //        8 
        TFT_DrawPoint(X-b,Y+a,fc);     //        5 
        TFT_DrawPoint(X+b,Y-a,fc);     //        1 
        TFT_DrawPoint(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        TFT_DrawPoint(X+a,Y+b,fc); 
        TFT_DrawPoint(X+a,Y+b,fc); 
        TFT_DrawPoint(X+a,Y-b,fc); 
        TFT_DrawPoint(X-a,Y-b,fc); 
        TFT_DrawPoint(X+b,Y+a,fc); 
        TFT_DrawPoint(X-b,Y+a,fc); 
        TFT_DrawPoint(X+b,Y-a,fc); 
        TFT_DrawPoint(X-b,Y-a,fc); 
    } 
	
} 
/*******************************************************************************
  * @brief  画线函数            
  * @param  None              
  * @retval None              
  * @Note   使用Bresenham 画线算法             
*******************************************************************************/
void TFT_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	

	TFT_SetXY(x0,y0);
	dx = x1-x0;       //计算x距离
	dy = y1-y0;       //计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)  //x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{             //且线的点数等于x距离，以x轴递增画点
		            // initialize error term
		error = dy2 - dx; 
		            // draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			                                 //画点
			TFT_DrawPoint(x0,y0,Color);
			                                 // test if error has overflowed
			if (error >= 0)                  //是否需要增加y坐标值
			{
				error-=dx2;
				                               // move to next line
				y0+=y_inc;                     //增加y坐标值
			}                                // end if error overflowed
			                                 // adjust the error term
			error+=dy2;
			                                 // move to the next pixel
			x0+=x_inc;                       //x坐标值每次画点后都递增1
		}                                  // end for
	}                                    // end if |slope| <= 1
	else                                 //y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{                                    //以y轴为递增画点
		                                   // initialize error term
		error = dx2 - dy; 
		                                   // draw the line
		for (index=0; index <= dy; index++)
		{
			                                 // set the pixel
			TFT_DrawPoint(x0,y0,Color);
			                                 // test if error overflowed
			if (error >= 0)
			{
				error-=dy2;
				                               // move to next line
				x0+=x_inc;
			}                                // end if error overflowed
			                                 // adjust the error term
			error+=dx2;
			                                 // move to the next pixel
			y0+=y_inc;
		}                                  // end for
	}                                    // end else |slope| > 1
}
/*******************************************************************************
  * @brief  画矩形函数            
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
{
	TFT_DrawLine(x,y,x+w,y,0xEF7D);
	TFT_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	TFT_DrawLine(x,y+h,x+w,y+h,0x2965);
	TFT_DrawLine(x,y,x,y+h,0xEF7D);
    TFT_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}
/*******************************************************************************
  * @brief  画矩形函数            
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
{
	if (mode==0)	{
		TFT_DrawLine(x,y,x+w,y,0xEF7D);
		TFT_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		TFT_DrawLine(x,y+h,x+w,y+h,0x2965);
		TFT_DrawLine(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		TFT_DrawLine(x,y,x+w,y,0x2965);
		TFT_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		TFT_DrawLine(x,y+h,x+w,y+h,0xEF7D);
		TFT_DrawLine(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		TFT_DrawLine(x,y,x+w,y,0xffff);
		TFT_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		TFT_DrawLine(x,y+h,x+w,y+h,0xffff);
		TFT_DrawLine(x,y,x,y+h,0xffff);
	}
}
/*******************************************************************************
  * @brief  在屏幕显示一凸起的按钮框         
  * @param  u16 x1,y1,x2,y2 按钮框左上角和右下角坐标           
  * @retval None              
  * @Note   None              
*******************************************************************************/
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	TFT_DrawLine(x1,  y1,  x2,y1, WHITE); //H
	TFT_DrawLine(x1,  y1,  x1,y2, WHITE); //V
	
	TFT_DrawLine(x1+1,y2-1,x2,y2-1, GRAY1);  //H
	TFT_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
	TFT_DrawLine(x2-1,y1+1,x2-1,y2, GRAY1);  //V
  TFT_DrawLine(x2  ,y1  ,x2,y2, GRAY2); //V
}
/*******************************************************************************
  * @brief  在屏幕显示一凹下的按钮框     
  * @param  u16 x1,y1,x2,y2 按钮框左上角和右下角坐标      
  * @retval None              
  * @Note   None              
*******************************************************************************/
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
	TFT_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
	TFT_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
	TFT_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
	TFT_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
	TFT_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
	TFT_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
}
/*******************************************************************************
  * @brief  DrawFont_GBK16  
  * @param  None       
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;

	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	TFT_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) TFT_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{
		

			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	TFT_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) TFT_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	TFT_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) TFT_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
		
	}
}
/*******************************************************************************
  * @brief  DrawFont_GBK24  
  * @param  None     
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) 
	{
		if( *s < 0x80 ) 
		{
			k=*s;
			if (k>32) k-=32; else k=0;

		    for(i=0;i<16;i++)
			for(j=0;j<8;j++) 
				{
			    	if(asc16[k*16+i]&(0x80>>j))	
					TFT_DrawPoint(x+j,y+i,fc);
					else 
					{
						if (fc!=bc) TFT_DrawPoint(x+j,y+i,bc);
					}
				}
			s++;x+=8;
		}
		else 
		{

			for (k=0;k<hz24_num;k++) 
			{
			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<24;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3]&(0x80>>j))
								TFT_DrawPoint(x+j,y+i,fc);
								else 
								{
									if (fc!=bc) TFT_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	TFT_DrawPoint(x+j+8,y+i,fc);
								else {
									if (fc!=bc) TFT_DrawPoint(x+j+8,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	
								TFT_DrawPoint(x+j+16,y+i,fc);
								else 
								{
									if (fc!=bc) TFT_DrawPoint(x+j+16,y+i,bc);
								}
							}
				    }
			  }
			}
			s+=2;x+=24;
		}
	}
}
/*******************************************************************************
  * @brief  DrawFont_Num32
  * @param  None   
  * @retval None              
  * @Note   None              
*******************************************************************************/
void TFT_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
{
	unsigned char i,j,k,c;
	//TFT_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
//	w=w/8;

    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	TFT_DrawPoint(x+j*8+k,y+i,fc);
				else {
					if (fc!=bc) TFT_DrawPoint(x+j*8+k,y+i,bc);
				}
			}
		}
	}
}

#ifdef SD_FONT
/*******************************************************************************
  * @brief  GetGBKCode_from_sd
  * @param  None   
  * @retval None              
  * @Note   None              
*******************************************************************************/
int GetGBKCode_from_sd(unsigned char* pBuffer,unsigned char * c)
 { 
	 
//char path0[512]="0:";
//char buffer[4096];   /* file copy buffer */
//uint8_t textFileBuffer[] = "中英文测试字符串 \r\nChinese and English test strings \r\n";
 unsigned char High8bit,Low8bit; 
 unsigned int pos; 
 printf("进入GetGBK函数");
 High8bit=*c; 
 Low8bit=*(c+1); 
 pos = ((High8bit-0xb0)*94+Low8bit-0xa1)*2*16 ;
 res= f_mount(0, &fs);
		 if(res != FR_OK){
 	printf("mount filesystem 0 failed : %d\n\r",res);
 }
	 printf("mount filesystem-ok");
	//写文件测试
	 printf("write file test......\n\r");
  res = f_open(&fdst , "0:/HZLIB.bin", FA_OPEN_EXISTING | FA_READ); 

	   
 if (  res == FR_OK ) 
 { 
	 printf("open file sucess");
  f_lseek (&fdst, pos);
  res = f_read( &fdst, pBuffer, 32,  &br ); //16*16大小的汉字其字模占用16*2个字节 
  f_close(&fdst); 
  return 0;
  } 
  else
	{
		printf("mount filesystem 0 failed : %d\n\r",res);
  return -1; 
	
	}
  }
 /*******************************************************************************
  * @brief  LCD_Char_CH
  * @param  None   
  * @retval None              
  * @Note   None              
*******************************************************************************/
void LCD_Char_CH(u16 x,u16 y,unsigned char * str,u16 Color,u16 bkColor) 
{ 
  u8 i,j,k; 
  u8 buffer[32]; 
  u16 tmp_char=0; 
  GetGBKCode_from_sd(buffer,str);             //取字模数据 
  for (k=0;k<32;k++) printf("%02X",buffer[k]);
  for (i=0;i<16;i++) 
  { 
    tmp_char=buffer[i*2]; 
    tmp_char=(tmp_char<<8); 
    tmp_char|=buffer[2*i+1]; 
    for (j=0;j<16;j++)
    { 
      if   ((tmp_char >> (15-j) & 0x01) == 0x01) 
      { 
        TFT_DrawPoint(x+j,y+i,Color); 
      } 
      else 
      { 
        TFT_DrawPoint(x+j,y+i,bkColor); 
      } 
    } 
  } 
 }

  /*******************************************************************************
  * @brief  LCD_Str_CH
  * @param  None   
  * @retval None              
  * @Note   None              
*******************************************************************************/
 void LCD_Str_CH(u16 x,u16 y,unsigned char *str,u16 Color,u16 bkColor)  
{
    while(*str != '\0')
    {
			if(x>(320-16))
			{	 
				x =0;        //换行
				y +=16;
					
			}
			if(y >(240-16))
			{
				 y =0;       //重新归零
				 x =0;
			}
	     LCD_Char_CH(x,y,str,Color,bkColor);      
       str += 2 ;
       x += 16 ;	
    }
}
 #endif
/*********************************END OF FILE**********************************/








