#include "M451Series.h"
#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"

volatile uint32_t g_u32AdcIntFlag;
int fftSize = 256; 
int ifftFlag = 0; 
int doBitReverse = 1; 
int datacount=0;
//int   g_u32AdcIntFlag=0;
float maxValue=0;
float maxValue2=0;
/* Reference index at which max energy of bin ocuurs */ 
uint32_t testIndex = 0;
arm_cfft_radix4_instance_f32 S; 
float32_t Mainfreq; 
int CalTime;
float32_t temp1[512] = {'\0'};
float32_t temp2[512] = {'\0'};
float32_t temp3[512] = {'\0'};
float32_t output1[256]={'\0'};
float32_t output2[256]={'\0'};
float32_t output3[256]={'\0'};
float32_t all1[256]={'\0'};
float32_t all2[256]={'\0'};
float32_t all3[256]={'\0'};

void TIMER0_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init TIMER0                                                                                             */
    /*---------------------------------------------------------------------------------------------------------*/
	SYS_ResetModule(TMR0_RST);

    /* Set timer0 periodic time-out period is 3us if timer clock is 12 MHz */
    TIMER_SET_CMP_VALUE(TIMER0, 512);//TIMER0->CMP = 36;

    /* Start timer counter in periodic mode and enable timer interrupt trigger EADC */
    TIMER0->CTL = TIMER_PERIODIC_MODE | TIMER_CTL_TRGEADC_Msk;
}

void EADCC()
{	
	EADC_Open(EADC,EADC_CTL_DIFFEN_SINGLE_END);
	printf("\nADC open\n");
	EADC_SetInternalSampleTime(EADC,6); //6 cycles
	EADC_ConfigSampleModule(EADC,4,EADC_TIMER0_TRIGGER,0); //MODULE4 CHANNEL 0
	EADC_ConfigSampleModule(EADC,3,EADC_TIMER0_TRIGGER,1); //MODULE4 CHANNEL 1
	EADC_ConfigSampleModule(EADC,2,EADC_TIMER0_TRIGGER,2); //MODULE4 CHANNEL 1
}

void FFT()
{
	int32_t i=0,j=0;
	int count = 0;
  int sum1=0,sum2=0,sum3=0;
	float32_t dat=0;
	
	GPIO_SetMode(PB,BIT0,GPIO_MODE_OUTPUT); //sensor analog pin
	GPIO_SetMode(PB,BIT1,GPIO_MODE_OUTPUT); //sensor analog pin
	GPIO_SetMode(PB,BIT2,GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT6, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT7, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PC, BIT3, GPIO_MODE_OUTPUT);
	
	arm_cfft_radix4_init_f32(&S, fftSize,ifftFlag, doBitReverse);
	
	while(1)
	{
		datacount=0;
		TIMER_Start(TIMER0);
	  while(1)
		{
			EADC_CLR_INT_FLAG(EADC,0x1); // CLEAR THE INTERRPUT FLAG FOR SAFE
			EADC_ENABLE_INT(EADC,0x1); //ENABLE INTERRUPT
			EADC_ENABLE_SAMPLE_MODULE_INT(EADC,0,4);//Enable sample module 4 interrupt
			EADC_ENABLE_SAMPLE_MODULE_INT(EADC,1,3);//Enable sample module 3 interrupt
			EADC_ENABLE_SAMPLE_MODULE_INT(EADC,2,2); //module 5
			NVIC_EnableIRQ(ADC00_IRQn);
			//g_u32AdcIntFlag = 0;
			EADC_START_CONV(EADC,4);
			EADC_START_CONV(EADC,3);
			EADC_START_CONV(EADC,2);
		
			while(g_u32AdcIntFlag == 0);
			//g_u32AdcIntFlag = 0;
			EADC_DISABLE_SAMPLE_MODULE_INT(EADC, 0,4);
			EADC_DISABLE_SAMPLE_MODULE_INT(EADC, 1,3);
			EADC_DISABLE_SAMPLE_MODULE_INT(EADC, 2,2);
		
			datacount++;
			temp1[(datacount-1)*2] = (float)EADC_GET_CONV_DATA(EADC,4)/4096;
			temp2[(datacount-1)*2] = (float)EADC_GET_CONV_DATA(EADC,3)/4096;
			temp3[(datacount-1)*2] = (float)EADC_GET_CONV_DATA(EADC,2)/4096;
			
			g_u32AdcIntFlag = 0;
			if(datacount > 256) 
			{
				break;
			}
		}
			
		TIMER_Stop(TIMER0);

		/* Process the data through the CFFT/CIFFT module */ 
		arm_cfft_radix4_f32(&S, temp1); 
		arm_cfft_radix4_f32(&S, temp2);
		arm_cfft_radix4_f32(&S, temp3);
		/* Process the data through the Complex Magnitude Module for calculating the magnitude at each bin */ 
		arm_cmplx_mag_f32(temp1, output1,fftSize);
		arm_cmplx_mag_f32(temp2, output2,fftSize);
		arm_cmplx_mag_f32(temp3, output3,fftSize);

		for(i=0;i<256;i++)
		{
			temp1[i*2+1]=0; //assign the odd order array to 0
			temp2[i*2+1]=0;
			temp3[i*2+1]=0;
		}
		
		sum1=sum1+output1[20];
		sum2=sum2+output2[20];
		sum3=sum3+output3[20];
		count++;
		
		if(count==120)
		{		
			printf("%d\t%d\t%d\t\n",sum1,sum2,sum3);
			if(sum1-sum2>0&&sum1-sum3>0) {
			PC6=0;
			PC7=0;
			PC3=1;
			}
			else if(sum2-sum1>0&&sum2-sum3>0) {
			PC7=1;
			PC6=0;
			PC3=0;
			}
			else if(sum3-sum1>0&&sum3-sum2>0) {
			PC3=0;
			PC6=1;
			PC7=0;
			}
			else {
			PC6=0;
			PC7=0;
			PC3=0;
			}
			
			
			
			if(sum1-sum2>0 && sum1-sum3>0 && sum1>5 && sum2-sum3>=0)
				PWM_ConfigOutputChannel(PWM0,2,50,10);
			else if(sum2-sum1>0 && sum2-sum3>0 && sum2>5)
				PWM_ConfigOutputChannel(PWM0,2,50,5);
			else if(sum3-sum1>0 && sum3-sum2>0 && sum3>5 && sum2-sum1>=0)
				PWM_ConfigOutputChannel(PWM0,2,50,25/10);	
			else
				PWM_ConfigOutputChannel(PWM0,2,50,0);
			sum1=0;
			sum2=0;
			sum3=0;
			count=0;
		}
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* ADC interrupt handler                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void ADC00_IRQHandler(void)
{
    g_u32AdcIntFlag = 1;
    EADC_CLR_INT_FLAG(EADC, 0x1); /* clear the A/D conversion flag */
}

void Correction(void)
{
		GPIO_SetMode(PC, BIT2, GPIO_MODE_OUTPUT); //pwm_output
		PWM_EnableOutput(PWM0, 0x04);
		PWM_Start(PWM0, 0x04);
	
	//EADC_Open(EADC,EADC_CTL_DIFFEN_SINGLE_END);
	//printf("ADC open\n");
	//EADC_SetInternalSampleTime(EADC,6); //6 cycles
	//EADC_ConfigSampleModule(EADC,4,EADC_ADINT0_TRIGGER,0); //MODULE4 CHANNEL 0
	//PWM_ConfigOutputChannel(PWM0,2,50,5);
	
		TIMER0_Init();
		EADCC();
		FFT();
	
	
		EADC_Close(EADC);
	
		SYS_ResetModule(EADC_RST);
    /* Disable ADC IP clock */
    CLK_DisableModuleClock(EADC_MODULE);

    /* Disable External Interrupt */
    NVIC_DisableIRQ(ADC00_IRQn);
		
    //printf("\nExit EADC sample code\n");
}
