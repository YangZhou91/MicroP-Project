/**
  ******************************************************************************
  * @file    TIM4_Init.c
  * @author  yan ge and ruofan wu
  * @version V4.1.0
  * @date    28-FEB-2014
  * @brief   Initiate the TIM4 for the specific use of PWM.
	
  ******************************************************************************
**/
/* includes -----------------------------------------------------------*/
#include "pitch_PWM_Timer.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


void TIM4_init(){
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_Tim4BaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOC and GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  /* GPIOC Configuration: TIM4 CH1 (PD12) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	
	/*TIM4 Initialization Structure*/
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 1000000) - 1;
	TIM_Tim4BaseStructure.TIM_Period = 19999; //  the larger the period, the more precise to control the motor angle
	TIM_Tim4BaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_Tim4BaseStructure.TIM_ClockDivision = 0;
	TIM_Tim4BaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_Tim4BaseStructure);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);// Enables TIM4 peripheral Preload register on ARR.
  
	/* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
	
}




