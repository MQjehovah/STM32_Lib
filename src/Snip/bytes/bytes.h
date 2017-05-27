/*******************************************************************************
  * @file                   bytes.h
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.3
  * @date                   2017.5.24
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
#ifndef  _BYTES_H
#define  _BYTES_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h" //提供u8等宏定义
/* Definition ----------------------------------------------------------------*/

/* Exported Functions --------------------------------------------------------*/
short byte2short(byte bytes[]);
int byte2int(byte bytes[]);
void short2Byte(short num,byte[] bytes, int offset);
void int2Byte(short num,byte[] bytes, int offset);

#endif
/*********************************END OF FILE**********************************/
