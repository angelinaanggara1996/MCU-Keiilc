#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define PLLCON_SETTING      CLK_PLLCON_72MHz_HXT
#define PLL_CLOCK           72000000

void SystemConfig(void);
void PrintSystemInfo(void);

//---------------------------------------------------//
void SYS_Init(void);
void EnableModuleClock(void);
void ModuleInit(void);

#ifdef __cplusplus
extern "C"
}
#endif

#endif //__SYSTEM_CONFIG_H__
