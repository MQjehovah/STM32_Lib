/*******************************************************************************
  * @file                   Bytes.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.5.24
  * @brief					提供一些字节转换操作
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Bytes.h"
/* Definition ----------------------------------------------------------------*/
#define Litter
/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  byte2short
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
short byte2short(byte bytes[])
{
	short tmp;
	tmp=byte[0];
	tmp<<8;
	tmp+=byte[0];
	return tmp;
}

int byte2int(byte bytes[])
{
	int tmp;
	tmp=byte[0];
	tmp<<8;
	tmp+=byte[0];
	tmp<<8;
	tmp+=byte[0];
	tmp<<8;
	tmp+=byte[0];
	return tmp;
}

void short2Byte(short num,byte[] bytes, int offset)
{
	bytes[offset]   = (byte)(num >> 8);
	bytes[offset+1] = (byte)(num);  
	return bytes;  
}  

void int2Byte(short num,byte[] bytes, int offset)
{
	bytes[offset]   = (byte)(num >> 8);
	bytes[offset+1] = (byte)(num);  
	return bytes;  
}

void float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到float的地址
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
#endif
/*********************************END OF FILE**********************************/
