#include "ADC_Driver.h"

void LM35_Init(){
	
	 /* enable clocks */
    SYSCTL_RCGCGPIO_R |= 0x10;      /* enable clock to GPIO_PORTE */
    SYSCTL_RCGCADC_R |= 1;          /* enable clock to ADC0 */
   	SYSCTL_RCGCWTIMER_R |= 1;    /* enable clock to WTimer Block 0 */
 
    /* initialize PE3 for AIN0 input */
    GPIO_PORTE_AFSEL_R |= 8;        /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~8;         /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 8;        /* enable analog function */
 
    /* initialize ADC0 */
    ADC0_ACTSS_R &= ~8;             /* disable SS3 during configuration */
    ADC0_EMUX_R &= ~0xF000;         /* software trigger conversion */
      ADC0_EMUX_R |= 0x5000;       /* timer trigger conversion seq 0 */  
	ADC0_SSMUX3_R = 0;              /* get input from channel 0 */
    ADC0_SSCTL3_R |= 6;             /* take one sample at a time, set flag at 1st sample */
    ADC0_IM_R |= (1<<3);  //unmask ADC0 seq 3 interrupt
		NVIC_EN0_R|= (1<<17);
		NVIC_PRI4_R |=(111<<13);
    ADC0_ACTSS_R |= 8;           /* enable ADC0 sequencer 3 */
	
   /* initialize wtimer 0 to trigger ADC at 1 sample/sec */
    WTIMER0_CTL_R = 0;           /* disable WTimer before initialization */
    WTIMER0_CFG_R = 0x04;        /* 32-bit option */
    WTIMER0_TAMR_R = 0x02;       /* periodic mode and down-counter */
    WTIMER0_TAILR_R = 16000000;  /* WTimer A interval load value reg (1 s) */
    WTIMER0_CTL_R |= 0x20;       /* timer triggers ADC */
    WTIMER0_CTL_R |= 0x01;       /* enable WTimer A after initialization */


}



int LM35_Value(){
		int Temp;
		unsigned int AdcValue;
			 //Adc value range from 0 to 4095
		//ref used is 3.3V
		AdcValue = ADC0_SSFIFO3_R;									//Read ADC
  	Temp =(( AdcValue * 330) / 4096);       //Temp in C
		return Temp;
}





