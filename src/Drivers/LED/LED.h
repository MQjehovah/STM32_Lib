/*******************************************************************************
  * @file                   LED.h
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016.3.17
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
#ifndef  _LED_H
#define  _LED_H

/* Includes ------------------------------------------------------------------*/

/* Definition ----------------------------------------------------------------*/
#define LED1_PORT     GPIOD
#define LED1_PIN      GPIO_Pin_13

#define LED2_PORT     GPIOG
#define LED2_PIN      GPIO_Pin_14

#define LED1_ON       GPIO_SetBits(LED1_PORT,LED1_PIN)
#define LED1_OFF      GPIO_ResetBits(LED1_PORT,LED1_PIN)

#define LED2_ON       GPIO_SetBits(LED2_PORT,LED2_PIN)
#define LED2_OFF      GPIO_ResetBits(LED2_PORT,LED2_PIN)

#define LED_ON(i)     LED##i##_ON
#define LED_OFF(i)    LED##i##_OFF

#define LED_NUM       2
/* Exported Functions --------------------------------------------------------*/
void led_init(void);
void led_flash(void);
#endif
/*********************************END OF FILE**********************************/
