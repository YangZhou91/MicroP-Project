  /*******************************************************************
  * @file    ADC_Impl.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    17-February-2014
  * @brief   This file provides functions to Initialize 
	*          the GPIO for ADC.
	********************************************************************/
	
#include <stdint.h>
#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"
#include <ADC_Impl.h>


void set_ADC_config(void){
	
//	Define ADC configuration
		ADC_InitTypeDef ADC_init_structure;
		ADC_CommonInitTypeDef ADC_Common_init_structure;
	
//	Define structure for the analog
		GPIO_InitTypeDef GPIO_init_structure;
	
//	1.
//	Enable the ADC interface clock.
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

//	2. ADC pins configuration
//  Enable the ADC GPIOs
		RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);
	  
		GPIO_StructInit(&GPIO_init_structure);
		GPIO_init_structure.GPIO_Pin = GPIO_Pin_4;
		GPIO_init_structure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_init_structure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_init_structure.GPIO_OType = GPIO_OType_PP;
		GPIO_init_structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
//	3. Enable the GPIO			
		GPIO_Init(GPIOA, &GPIO_init_structure);
		
//	Sets the selected data port bits.
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_WriteBit (GPIOA, GPIO_Pin_4, Bit_SET);
		GPIO_Write (GPIOA, 0x16);
//	3. Configure the ADC Prescaler
		
		ADC_Common_init_structure.ADC_Mode = ADC_Mode_Independent;
		ADC_Common_init_structure.ADC_Prescaler = ADC_Prescaler_Div2;
		ADC_Common_init_structure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
		ADC_Common_init_structure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInit(&ADC_Common_init_structure);
		
		ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;
		ADC_init_structure.ADC_ScanConvMode = DISABLE;
		ADC_init_structure.ADC_ContinuousConvMode = DISABLE;
		ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_init_structure.ADC_NbrOfConversion = 1;
		ADC_Init(ADC1, &ADC_init_structure);
		ADC_Cmd(ADC1, ENABLE); //Enable Module
		ADC_TempSensorVrefintCmd(ENABLE);
		
		ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_144Cycles); 
		
		return;
}

uint16_t get_ADC_temp(void){
		
		ADC_SoftwareStartConv(ADC1);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		ADC_GetConversionValue(ADC1);
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
		uint16_t temperature = ADC_GetConversionValue(ADC1);
		return temperature;
}
