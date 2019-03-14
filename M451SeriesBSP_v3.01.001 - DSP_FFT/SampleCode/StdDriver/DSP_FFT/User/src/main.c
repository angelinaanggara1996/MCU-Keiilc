/****************************************************************************
 * @file     main.c
 * @version  V1.0
 * $Date: 16/09/22 10:04a $
 * @brief
 *         Display how to use DSP FFT function
 *     		
 * @note
 * Copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include "main.h"

#define TEST_LENGTH_SAMPLES 2048 //Must be fftsize*2
 
/* ------------------------------------------------------------------- 
* External Input and Output buffer Declarations for FFT Bin Example 
* ------------------------------------------------------------------- */ 
//extern float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES]; 
//static float32_t testOutput[TEST_LENGTH_SAMPLES/2]; 

/* ------------------------------------------------------------------ 
* Global variables for FFT Bin Example 
* ------------------------------------------------------------------- */ 
//uint32_t fftSize = 1024; 
//uint32_t ifftFlag = 0; 
//uint32_t doBitReverse = 1; 
/* Reference index at which max energy of bin ocuurs */ 
//uint32_t refIndex = 213, testIndex = 0; 
//arm_cfft_radix4_instance_f32 S; 
//float32_t maxValue,Mainfreq; 
//int CalTime;

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
    /* Init System, peripheral clock and multi-function I/O */
    SystemConfig();
	
		Correction();
	 
    while(1);      
	 
}

