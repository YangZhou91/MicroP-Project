/**
  ******************************************************************************
  * @file    pitch_motor.c
  * @author  yan ge
  * @version V4.1.0
  * @date    28-FEB-2014
  * @brief   
	* @note 	Pitch PWM is using TIM4. 
						Output Pin is PD12
	
  ******************************************************************************
**/
	
/* includes -----------------------------------------------------------*/
#include "pitch_motor.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

	/**
  * @brief  pitch_motor (int)
  * @param  int CCR1_val : control duty cycle of a PWM signal
  * @retval None
  */
void pitch_motor(int CCR1_Val){
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  /* PWM21 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
}
