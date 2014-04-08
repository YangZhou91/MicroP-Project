/**
  ******************************************************************************
  * @file    roll_motor.c
  * @author  yan ge
  * @version V4.1.0
  * @date    28-FEB-2014
  * @brief   
	* @note 	ROll PWM is using TIM3.
						Output Pin is PC6.
	
  ******************************************************************************
**/
	
/* includes -----------------------------------------------------------*/
#include "roll_motor.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

	/**
  * @brief  Rol_Motor (int)
  * @param  int CCR1_val : control duty cycle of a PWM signal
  * @retval None
  */
void roll_motor(int CCR1_Val){
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
}
