/**
  ******************************************************************************
  * @file    roll_PWM_Timer.c
  * @author  yan ge and ruofan wu
  * @version V4.1.0
  * @date    28-FEB-2014
  * @brief   Initiate the TIM3 for the specific use of PWM.
	
  ******************************************************************************
**/
/* includes -----------------------------------------------------------*/
#include "roll_PWM_Timer.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

void roll_TIM3_init(){
	
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOC and GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* GPIOC Configuration: TIM3 CH1 (PC6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	
	/*TIM3 Initialization Structure*/
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 1000000) - 1;
	TIM_TimeBaseStructure.TIM_Period = 19999; //  the larger the period, the more precise to control the motor angle
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);// Enables TIM3 peripheral Preload register on ARR.
  
	/* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
	
}




