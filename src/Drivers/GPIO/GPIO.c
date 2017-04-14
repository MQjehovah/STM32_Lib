/*******************************************************************************
  * @file                   GPIO.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.3.17
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "GPIO.h"
/* Definition ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  GPIO端口配置       
  * @param  None              
  * @retval None              
  * @Note   None              
*******************************************************************************/
void gpio_config(GPIO_TypeDef * PORT,uint16_t PIN,GPIOMode_TypeDef MODE)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = PIN ;
	GPIO_InitStructure.GPIO_Mode = MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT,&GPIO_InitStructure);
}

/*********************************END OF FILE**********************************/
