#ifndef __ANALOG_H__
#define __ANALOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "M451Series.h"

void GPAB_IRQHandler(void);
void AdcSingleModeTest(void);
void ADC_IRQHandler(void);
void Correction(void);

#ifdef __cplusplus
extern "C"
}
#endif
#endif //__ANALOG_H__
