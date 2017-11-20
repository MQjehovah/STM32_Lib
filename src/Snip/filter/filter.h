/*******************************************************************************
  * @file                   filter.h
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2017.5.25
  * @brief                  
  ******************************************************************************
  * @attention 
*******************************************************************************/
#ifndef  _FILTER_H
#define  _FILTER_H

/* Includes ------------------------------------------------------------------*/

/* Definition ----------------------------------------------------------------*/
#define N 12
/* Exported Functions --------------------------------------------------------*/
double KalmanFilter(const double ResrcData, double ProcessNiose_Q, double MeasureNoise_R);


#endif
/*********************************END OF FILE**********************************/
