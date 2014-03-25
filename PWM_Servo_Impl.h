  /*******************************************************************
  * @file    PWM_Servo_Impl.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    9-March-2014
  * @brief   This file provides functions to manage the following 
  *          functionalities:
  *           - Initialization and Configuration of PWM
	********************************************************************/
	
/*
 * Function: PWM_Servo_config
 * ----------------------------
 * Configures PWM OC mode channel-1 TIM3
 *   Returns: void
 *
 *   @param: none
 *   returns: none
 */
void PWM_Servo_config(void);


/*
 * Function: PWM_Display
 * ----------------------------
 * Computes new DutyCycle for a new tilt angle
 *   Returns: void
 *
 *   @param: float
 *   returns: none
 */
void PWM_Display(float angle);

