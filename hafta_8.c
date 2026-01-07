#include "stdint.h"
#include "stdbool.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
/*
Ödev: ADC'den gelen veriyi LCD'nin sağına yazdır.Potansiyometreden veri al.(PE3)
Sharpdevelop indir.5-6. version. 
*/

//Function Prototypes.
void init();
void ADC_Handler();

uint32_t ADC_Buffer[4] = {0};

int main(void)
{
    init();
    
    while(1){

        //ADCProcessorTrigger(ADC0_BASE, 1);
        //while(!ADCIntStatus(ADC0_BASE, 1,  false));
        //ADCSequenceDataGet(ADC0_BASE,  1,  ADC_Buffer);
        //ADCIntClear(ADC0_BASE, 1);         
    }
}

void init(){

    IntMasterEnable();

    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    IntEnable(INT_ADC0SS1);

    //Tek çevrim mod olarak sequencer 1 ayarlandı.
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR,  0);
    
    ADCSequenceStepConfigure( ADC0_BASE,  1,  0,  ADC_CTL_TS);
    ADCSequenceStepConfigure( ADC0_BASE,  1,  1,  ADC_CTL_TS);
    ADCSequenceStepConfigure( ADC0_BASE,  1,  2,  ADC_CTL_TS);
    ADCSequenceStepConfigure( ADC0_BASE,  1,  3,  ADC_CTL_TS | ADC_CTL_IE |ADC_CTL_END);

    ADCIntEnable(ADC0_BASE, 1);
    ADCIntRegister(ADC0_BASE,  1, ADC_Handler);

    ADCSequenceEnable(ADC0_BASE,  1);

    ADCProcessorTrigger(ADC0_BASE, 1);
}

void ADC_Handler(){

    ADCSequenceDataGet(ADC0_BASE,  1,  ADC_Buffer);

    ADCIntClear(ADC0_BASE, 1);

    ADCProcessorTrigger(ADC0_BASE, 1);
}
