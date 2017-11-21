/*******************************************************************************
  * @file                   ring_buffer.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016/07/26  
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/

/* Definition ----------------------------------------------------------------*/
struct ring_buffer rx_buffer = {{0},0,0,0};
/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
  * @brief  环形缓冲区写一个字节              
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void RingBuf_write(unsigned char c)
{
    int i = (unsigned int)(rx_buffer.tail + 1) % RX_BUFFER_SIZE;

    if (i != rx_buffer.head) 
    {
        rx_buffer.buffer[rx_buffer.tail] = c;
        rx_buffer.tail = i;
        rx_buffer.lenth++;
    }
}
/*******************************************************************************
  * @brief  环形缓冲区读一个字节              
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
unsigned char RingBuf_read()
{
    unsigned char c;
    if (rx_buffer.head == rx_buffer.tail) 
    {
        return 0;
    } 
    else 
    {
        c = rx_buffer.buffer[rx_buffer.head];
        rx_buffer.head = (unsigned int)(rx_buffer.head + 1) % RX_BUFFER_SIZE;
        rx_buffer.lenth--;
        return c;
    }
}
/*******************************************************************************
  * @brief  缓冲区清空              
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void RingBuf_flush()
{
    int i;
    for(i=0;i<RX_BUFFER_SIZE;i++)
    {
        ring_buffer.buffer[i]=0;
    }
    rx_buffer.head=0;
    rx_buffer.tail=0;
    rx_buffer.lenth=0;
}

/*********************************END OF FILE**********************************/
