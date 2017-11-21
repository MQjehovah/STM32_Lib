/*******************************************************************************
  * @file                   ring_buffer.h
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016/07/26
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
#ifndef  _RING_BUFFER_H
#define  _RING_BUFFER_H

/* Includes ------------------------------------------------------------------*/

/* Definition ----------------------------------------------------------------*/
#define RX_BUFFER_SIZE        64

struct ring_buffer 
{
  unsigned char buffer[RX_BUFFER_SIZE];
  unsigned char head;
  unsigned char tail;
  unsigned long lenth;
};
/* Exported Functions --------------------------------------------------------*/
#endif
/*********************************END OF FILE**********************************/

