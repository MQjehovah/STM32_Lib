/*******************************************************************************
  * @file                   LED.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.3.17
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "LED.h"
/* Definition ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  LED端口配置       
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void led_gpio_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
#if(LED_NUM>0)
	GPIO_InitStructure.GPIO_Pin = LED1_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_PORT,&GPIO_InitStructure);
#endif
#if(LED_NUM>1)
	GPIO_InitStructure.GPIO_Pin = LED2_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED2_PORT,&GPIO_InitStructure);
#endif
}
/*******************************************************************************
  * @brief  LED初始化              
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void led_init()
{
	led_gpio_config();
}

/*******************************************************************************
  * @brief  LED闪烁              
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void led_flash(unsigned char num,int time,int count)
{
	int i = 0;
	for(i=0;i<count;i++)
	{
		LED_ON(num);
		delay_us(time);
		LED_OFF(num);
		delay_us(time);
	}
}

/*******************************************************************************
  * @brief  LED闪烁              
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void led_flash_os(unsigned char num,int time,int times)
{
	int i = 0;
	for(i=0;i<count;i++)
	{
		LED_ON(num);
		OSTimeDlyHMSM(0, 0, 1, 0);
		LED_OFF(num);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
/*********************************END OF FILE**********************************/
