/*******************************************************************************
  * @file                   FFT.c
  * @Author:                MQjehovah                 mail:MQjehovah@hotmail.com
  * @version                1.0.0
  * @date                   2016/06/07
  * @brief
  ******************************************************************************
  * @attention
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/

/* Definition ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/*******************************************************************************
  * @brief  None
  * @param  None
  * @retval None
  * @Note   None
*******************************************************************************/
#include "fft.h"
#include <stdlib.h>

complex WN0 = {1, 0};
complex WN1 = {0.7109, -0.7109};
complex WN2 = {0, -1};
complex WN3 = { -0.7109, -0.7109};

complex ComplexMul( complex c1, complex c2 )
{
    complex r;

    r.re = c1.re * c2.re - c1.im * c2.im; //Re
    r.im = c1.re * c2.im + c1.im * c2.re; //Im

    return r;
}

complex ComplexAdd( complex c1, complex c2 )
{
    complex r;

    r.re = c1.re + c2.re;
    r.im = c1.im + c2.im;

    return r;
}

complex ReverseComplex( complex c )
{
    c.re = -c.re;
    c.im = -c.im;

    return c;
}

/*
 * 8���-2ʱ��FFT�㷨
 */
void fft( complex* x, complex* r )
{
    complex temp1[8];
    complex temp2[8];

    temp1[0] = x[0];
    temp1[1] = ComplexMul( x[1], WN0 );
    temp1[2] = temp1[2] = x[2];
    temp1[3] = ComplexMul( x[3], WN0 );
    temp1[4] = x[4];
    temp1[5] = ComplexMul( x[5], WN0 );
    temp1[6] = x[6];
    temp1[7] = ComplexMul( x[7], WN0 );

    temp2[0] = ComplexAdd( temp1[0], temp1[1] );
    temp2[1] = ComplexAdd( temp1[0], ReverseComplex( temp1[1] ) );
    temp2[2] = ComplexAdd( temp1[2], temp1[3] );
    temp2[3] = ComplexAdd( temp1[2], ReverseComplex( temp1[3] ) );
    temp2[4] = ComplexAdd( temp1[4], temp1[5] );
    temp2[5] = ComplexAdd( temp1[4], ReverseComplex( temp1[5] ) );
    temp2[6] = ComplexAdd( temp1[6], temp1[7] );
    temp2[7] = ComplexAdd( temp1[6], ReverseComplex( temp1[7] ) );

    temp2[2] = ComplexMul( temp2[2], WN0 );
    temp2[3] = ComplexMul( temp2[3], WN2 );
    temp2[6] = ComplexMul( temp2[6], WN0 );
    temp2[7] = ComplexMul( temp2[7], WN2 );

    temp1[0] = ComplexAdd( temp2[0], temp2[2] );
    temp1[1] = ComplexAdd( temp2[1], temp2[3] );
    temp1[2] = ComplexAdd( temp2[0], ReverseComplex( temp2[2] ) );
    temp1[3] = ComplexAdd( temp2[1], ReverseComplex( temp2[3] ) );
    temp1[4] = ComplexAdd( temp2[4], temp2[6] );
    temp1[5] = ComplexAdd( temp2[5], temp2[7] );
    temp1[6] = ComplexAdd( temp2[4], ReverseComplex( temp2[6] ) );
    temp1[7] = ComplexAdd( temp2[5], ReverseComplex( temp2[7] ) );

    temp1[4] = ComplexMul( temp1[4], WN0 );
    temp1[5] = ComplexMul( temp1[5], WN1 );
    temp1[6] = ComplexMul( temp1[6], WN2 );
    temp1[7] = ComplexMul( temp1[7], WN3 );

    r[0] = ComplexAdd( temp1[0], temp1[4] );
    r[1] = ComplexAdd( temp1[1], temp1[5] );
    r[2] = ComplexAdd( temp1[2], temp1[6] );
    r[3] = ComplexAdd( temp1[3], temp1[7] );
    r[4] = ComplexAdd( temp1[0], ReverseComplex( temp1[4] ) );
    r[5] = ComplexAdd( temp1[1], ReverseComplex( temp1[5] ) );
    r[6] = ComplexAdd( temp1[2], ReverseComplex( temp1[6] ) );
    r[7] = ComplexAdd( temp1[3], ReverseComplex( temp1[7] ) );
}

/*
 * Bit Reverse
 * === Input ===
 * x : complex numbers
 * n : nodes of FFT. @N should be power of 2, that is 2^(*)
 * l : count by bit of binary format, @l=CEIL{log2(n)}
 * === Output ===
 * r : results after reversed.
 * Note: I use a local variable @temp that result @r can be set
 * to @x and won't overlap.
 */
void BitReverse( complex* x, complex* r, int n, int l )
{
    int i = 0;
    int j = 0;
    short stk = 0;
    static complex* temp = 0;

    temp = ( complex* )malloc( sizeof( complex ) * n );
    if ( !temp )
    {
        return;
    }

    for ( i = 0; i < n; i++ )
    {
        stk = 0;
        j = 0;
        do
        {
            stk |= ( i >> ( j++ ) ) & 0x01;
            if ( j < l )
            {
                stk <<= 1;
            }
        }
        while ( j < l );

        if ( stk < n )            /* ���㵹λ����� */
        {
            temp[stk] = x[i];
        }
    }
    /* copy @temp to @r */
    for ( i = 0; i < n; i++ )
    {
        r[i] = temp[i];
    }
    free( temp );
}


/*********************************END OF FILE**********************************/
