#include "SystemConfig.h"
#include "M451Series.h"
#include "stdio.h"
#include "GPIOConfig.h"


void SystemConfig(void)
{
		/* Unlock protected registers */
    SYS_UnlockReg();

    SYS_Init();

    EnableModuleClock();

    GpioRegisterConfig();

		/* Lock protected registers */
    SYS_LockReg();

    ModuleInit();
		
}

void SYS_Init(void)
{
	   /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable HXT */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL and SysTick source to HCLK/2*/
    CLK_SetCoreClock(12000000);
    CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLKSEL_HCLK_DIV2);
}

void EnableModuleClock(void)
{
	 /* Enable peripheral clock */
		CLK_EnableModuleClock(EADC_MODULE);
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(TMR0_MODULE);
		CLK_EnableModuleClock(TMR1_MODULE);
		CLK_EnableModuleClock(PWM0_MODULE);
	
    /* Peripheral clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, CLK_CLKDIV0_UART(1));
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, 0);
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_HXT, 0);
	
	
		CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));
		CLK_SetModuleClock(PWM0_MODULE,CLK_CLKSEL2_PWM0SEL_PCLK0,0);

    /* PWM clock frequency can be set equal or double to HCLK by choosing case 1 or case 2 */
    /* case 1.PWM clock frequency is set equal to HCLK: select PWM module clock source as PCLK */
    //CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PCLK1, NULL);

}

void ModuleInit(void)
{
    /*--------------------------------------------------------------------------------------*/
    /*---------------------------------Init UART0------------------------------------------ */
    /*--------------------------------------------------------------------------------------*/
    /* Set PD multi-function pins for UART0 RXD and TXD */
    SYS->GPD_MFPL = SYS_GPD_MFPL_PD0MFP_UART0_RXD | SYS_GPD_MFPL_PD1MFP_UART0_TXD;
	
		/* Reset IP */
    SYS_ResetModule(UART0_RST);
		SYS_ResetModule(PWM0_RST);
	
    /* Configure UART0 and set UART0 Baudrate */
    UART_Open(UART0, 115200);

    /*--------------------------------------------------------------------------------------*/
    /*---------------------------------Init I2C0------------------------------------------- */
    /*--------------------------------------------------------------------------------------*/

    /* Open I2C module and set bus clock */
    //I2C_Open(I2C0, 100000);

    /* Get I2C0 Bus Clock */
    //printf("I2C clock %d Hz\n", I2C_GetBusClockFreq(I2C0));

    /* Set I2C 4 Slave Addresses */
    //I2C_SetSlaveAddr(I2C0, 0, 0x15, 0);   /* Slave Address : 0x15 */
    //I2C_SetSlaveAddr(I2C0, 1, 0x35, 0);   /* Slave Address : 0x35 */
    //I2C_SetSlaveAddr(I2C0, 2, 0x55, 0);   /* Slave Address : 0x55 */
    //I2C_SetSlaveAddr(I2C0, 3, 0x75, 0);   /* Slave Address : 0x75 */

    /* Enable I2C interrupt */
    //I2C_EnableInt(I2C0);
    //NVIC_EnableIRQ(I2C0_IRQn);
}

void PrintSystemInfo(void)
{
    printf("\n");
    printf("+---------------------------------------------------------+ \n");
    printf("---------------------NCKU DIC LAB\n---------------------------");
    printf(" CPU Frenquency : @ %d.%dMHz\n", SystemCoreClock/1000000,SystemCoreClock%1000000);
    printf(" Create Date    : %s At %s\r\n", __DATE__, __TIME__);
    printf("+---------------------------------------------------------+ \n");
  	printf("\n\n");
}
