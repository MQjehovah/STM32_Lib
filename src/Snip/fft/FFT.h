/*******************************************************************************
  * @file                   FFT.h
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016/06/07
  * @brief                  
  ******************************************************************************
  * @attention
*******************************************************************************/
#ifndef  _FFT_H
#define  _FFT_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
/* Definition ----------------------------------------------------------------*/
typedef unsigned int  uint16u;
typedef unsigned char uint8;
typedef unsigned long uint32;
typedef int           sint16;
typedef char          sint8;
typedef long          sint32;
typedef float         fp32;
typedef double        fp64;

#define FFT_N               (8)  //8点FFT                  

typedef struct Complex
{
	fp32 re;
	fp32 im;
} complex;  //复数  FFT中的一个节点单元



/* Exported Functions --------------------------------------------------------*/
complex ComplexMul(complex c1, complex c2);
complex ComplexAdd(complex c1, complex c2);
complex ReverseComplex(complex c);
void fft(complex *x, complex *r);
void BitReverse(complex *x, complex *r, int n, int l);


#endif
/*********************************END OF FILE**********************************/

