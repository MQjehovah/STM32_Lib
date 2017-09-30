/*******************************************************************************
  * @file                   USART.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.3.18
  * @brief
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "USART.h"
#include "protocol.h"
/* Definition ----------------------------------------------------------------*/
#define Debug_USART        USART1
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
    set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


GPIO_DEV * RS485EN = NULL;
/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  USART管脚配置
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_GPIO_Config(USART_TypeDef *USARTx)
{
    if(USARTx == USART1)
    {
        GPIO_Config(&PA9, GPIO_Mode_AF_PP);
        GPIO_Config(&PA10, GPIO_Mode_IN_FLOATING);
    }
    if(USARTx == USART2)
    {
        GPIO_Config(&PA2, GPIO_Mode_AF_PP);
        GPIO_Config(&PA3, GPIO_Mode_IN_FLOATING);
    }
    if(USARTx == USART3)
    {
        GPIO_Config(&PB10, GPIO_Mode_AF_PP);
        GPIO_Config(&PB11, GPIO_Mode_IN_FLOATING);
    }
//	if(USARTx == USART4)
//	{
//		GPIO_Config(&PA9,GPIO_Mode_AF_PP);
//		GPIO_Config(&PA10,GPIO_Mode_IN_FLOATING);
//	}
//	if(USARTx == USART5)
//	{
//		GPIO_Config(&PA9,GPIO_Mode_AF_PP);
//		GPIO_Config(&PA10,GPIO_Mode_IN_FLOATING);
//	}
}
/*******************************************************************************
  * @brief  USART初始化
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_Config(USART_TypeDef *USARTx, u16 Buand)
{
    USART_InitTypeDef USART_InitStructure;

    USART_GPIO_Config(USARTx);
    USART_InitStructure.USART_BaudRate = Buand;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//使能接受中断，在接受移位 寄存器中有数据是产生
    //USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//使能发送中断，在发送完数据 后产生。
    USART_Cmd(USARTx, ENABLE);
}

/*******************************************************************************
  * @brief  485初始化
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_485_Config(USART_TypeDef *USARTx, u16 Buand, GPIO_DEV *ENPIN)
{
    RS485EN = ENPIN;
    GPIO_Config(RS485EN, GPIO_Mode_Out_PP);
    GPIO_WritePin(RS485EN, LOW);
    USART_Config(USARTx, Buand);
}
/*******************************************************************************
  * @brief  USART发送一个字符
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_SendChar(USART_TypeDef *USARTx, char Data)
{
    USART_SendData(USARTx, Data);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}
/*******************************************************************************
  * @brief  485发送一个字符
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_485_SendChar(USART_TypeDef *USARTx, char Data)
{
    GPIO_WritePin(RS485EN, HIGH);
    USART_SendChar(USARTx, Data);
    GPIO_WritePin(RS485EN, LOW);
}
/*******************************************************************************
  * @brief  USART发送字符串
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_SendStr(USART_TypeDef *USARTx, char* senddata)
{
    int lenth = strlen(senddata);
    int i;
    for (i = 0; i < lenth + 1; i++)
    {
        USART_SendData(USARTx, *senddata++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}
/*******************************************************************************
  * @brief  485发送字符串
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_485_SendStr(USART_TypeDef *USARTx, char* senddata)
{
    GPIO_WritePin(RS485EN, HIGH);
    USART_SendStr(USARTx, senddata);
    GPIO_WritePin(RS485EN, LOW);
}
/*******************************************************************************
  * @brief  串口发送数组
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_SendArray(USART_TypeDef *USARTx, u8 data[], u16 count)
{
    u16 i = 0;
    for(i = 0; i < count; i++)
        USART_SendChar(USARTx, data[i]);
}
/*******************************************************************************
  * @brief  485发送数组
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART_485_SendArray(USART_TypeDef *USARTx, u8 data[], u16 count)
{
    GPIO_WritePin(RS485EN, HIGH);
    USART_SendArray(USARTx, data, count);
    GPIO_WritePin(RS485EN, LOW);
}
/*******************************************************************************
  * @brief  USART_PrintNumber
  * @param  base：进制
  * @retval None
  * @Note   按指定进制输出数字
*******************************************************************************/
void USART_PrintNumber(USART_TypeDef *USARTx, unsigned long n, u8 base)
{
    unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars.
    unsigned long i = 0;

    if (n == 0)
    {
        USART_485_SendChar(USARTx, '0');
        return;
    }

    while (n > 0)
    {
        buf[i++] = n % base;
        n /= base;
    }

    for (; i > 0; i--)
        USART_485_SendChar(USARTx, 
		(char) (buf[i - 1] < 10 ?'0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}

/*******************************************************************************
  * @brief  串口1中断
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//如果寄存器中有数据
    {
        u8 data = USART_ReceiveData(USART1);
		Parser(data);
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_TC);
    }
}


//不使用半主机模式
#if 1 //如果没有这段，则需要在target选项中选择使用USE microLIB
#pragma import(__use_no_semihosting)
struct __FILE
{
    int handle;
};
FILE __stdout;

void _sys_exit(int x)
{
    x = x;
}
#endif


PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(Debug_USART, (u8)ch);
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(Debug_USART, USART_FLAG_TXE) == RESET);

    return ch;
}
/*********************************END OF FILE**********************************/
