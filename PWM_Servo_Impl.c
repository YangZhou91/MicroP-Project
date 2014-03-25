  /*******************************************************************
  * @file    PWM_Servo_Impl.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    9-March-2014
  * @brief   This file provides functions to manage the following 
  *          functionalities:
  *           - Initialization and Configuration of PWM
	********************************************************************/
	


#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4_discovery_lis302dl.h"
#include "misc.h"
#include "PWM_Servo_Impl.h"


/*
 * Function: PWM_Servo_config
 * ----------------------------
 * Configures PWM OC mode channel-1 TIM3
 *   Returns: void
 *
 *   @param: none
 *   returns: none
 */
void PWM_Servo_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOC clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* GPIOC Configuration: TIM3 CH1 (PC6)*/
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStruct); 
	
	/* Connect TIM3 pins to AF2 */  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	
	
	// PWM and TIM
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	uint16_t PrescalerValue = 4000;
	
	/* Compute the prescaler value */
 //  CounterClkFreq = (SystemCoreClock/2) / 4000 = 21KHz
 //  each counter clock pulse = 1/21KHz = 0.0476ms
 //  to obtain 20ms pulse choose TIM_Period to be 420* 0.0476ms = 20ms
 
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 420;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
	
	return;
}


/*
 * Function: PWM_Display
 * ----------------------------
 * Computes new DutyCycle for a new tilt angle
 *   Returns: void
 *
 *   @param: float
 *   returns: none
 */
void PWM_Display(float angle)
{
	
	//Desired dutycycle upper bounds used in linear interpolation
	uint32_t angle_neg90 = 15;
	uint32_t zero = 35;
	int32_t angleInt = angle;
	
	int32_t angle_zero = 0;
	float sign = 0, angle_neg, angle_pos, constant = (float)2/(float)9;
	
	
	if (angleInt == angle_zero)
	{
		TIM3->CCR1 = 35;
	}
	else if (angle < sign)
	{
		angle = angle*(float)(-1);
		//linear interpolation
		angle_neg = (float)angle_neg90 + ((float)angle_neg90-(((float)angle_neg90*angle)/(float)90));
		TIM3->CCR1 =  (uint32_t)angle_neg;
	}
	else
	{
		//Negative angle
		angle_pos = (float)zero + constant*angle;
		TIM3->CCR1 =  (uint32_t)angle_pos;
	}
	
	return;
	
}
