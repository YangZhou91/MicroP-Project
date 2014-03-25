  /*******************************************************************
  * @file    SevenSegLED.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    9-March-2014
  * @brief   This file provides functions to manage the following 
  *          functionalities:
  *           - Initialization and Configuration of LED GPIOs
  *           - Display floating point number on 7 segment
  *           - Display temperature
  *           - Turn PWM LED on/off
  *           - Trigger Alarm through PWM mechanism
  *           - Delay countdown counter
	********************************************************************/

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"
#include <SevenSegLED.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_conf.h"

/*
 * Function: LED_GPIO_Config
 * ----------------------------
 * Configures GPIO port D pins(0 - 10, 14) for 7-segment and PWM LED 
 *   Returns: void
 *
 *   @param: void
 */
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure_D;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure_D);
	GPIO_InitStructure_D.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_3 | GPIO_Pin_4  | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure_D.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure_D.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure_D.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_D.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure_D);
}


/*
 * Function: SevenSegLED
 * ----------------------------
 * Displays single digit number in a segment 
 *   Returns: void
 *
 *   @param: int number - Number to display (0 - 9) and 10 for Dot
 *   @param: int position - segment (1, 2, 3) 
 *
 *   returns: void
 */
void SevenSegLED(int number,  int position)
{
	
	// reject out of bounds number and position
	if ((number < 0) || (number > 12) || (position < 1) || (position > 3))
		return;
	
	// Select line implementation
	if (position == SEGMENT_1)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_8, Bit_SET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_RESET);
	}
	else if (position == SEGMENT_2)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_SET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_SET);
	}
	
	// Display number 0 -> 9 and dot
	switch(number)
	{
		case 0:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_SET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		
		  break;
		
		case 1:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_SET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		  break;
		
		case 2:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_SET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_RESET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		break;
		
		case 3:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_SET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		break;
		
		case 4:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_SET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		break;
		
		case 5:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_RESET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		  break;
		
		case 6:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_RESET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		  break;
		
		case 7:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_SET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		  break;
		
		case 8:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_SET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		  break;
		
		case 9:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_SET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_SET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_SET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		  break;
		
		//CASE 10 DOT
		case 10:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_RESET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_RESET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_SET);
		
		  break;
		
		case 11:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_RESET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_SET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_RESET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		  break;
		
		case 12:
			GPIO_WriteBit (GPIOD, GPIO_Pin_0, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_1, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_2, Bit_RESET);
		
			GPIO_WriteBit (GPIOD, GPIO_Pin_3, Bit_RESET);
			GPIO_WriteBit (GPIOD, GPIO_Pin_4, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_5, Bit_RESET);
		
		  GPIO_WriteBit (GPIOD, GPIO_Pin_6, Bit_RESET);	
		  GPIO_WriteBit (GPIOD, GPIO_Pin_7, Bit_RESET);
		
		default:
			return;
	}
	
	return;
}

/*
 * Function: Delay
 * ----------------------------
 * Countdown timer
 *   Returns: void
 *
 *   @param: uint32_t nCount - countds down from this value
 *   returns: void
 */
void Delay(uint32_t nCount) 
{
  while(nCount--);
}


/*
 * Function: DisplayTemperature
 * ----------------------------
 * Breaks the floating point temperature into four segments
 *
 *   @param: float temperature - temperature in float
 *   returns: void
 */
void DisplayTemperature(float temperature)
{
	
	//init the array
	int temp[4] = {0, 0, 0, 0};
	int div = 10;
	int div_100 = 100;
	float ten=10.0;
	int temp_cast = 0;
	
	// reject less than zero temperature 
	if (temperature < 0)
	{
		printf("Tmeperature:%f is less then zero.\n", temperature);
		return;
	}
	
	if (temperature < 100)
	{
		temp[0] = (int)(temperature / div);
		temp[1] = (int)(temperature - (temp[0])*div);
		temp[2] = 10; // dot
		temp_cast = (int)temperature;
		temp[3] = (temperature - (float)temp_cast)*ten;
		
		SevenSegLED(temp[0], SEGMENT_1);
	  Delay(SEGMENT_DELAY);
	
		SevenSegLED(temp[1], SEGMENT_2);
		Delay(SEGMENT_DELAY);
		
		SevenSegLED(temp[2], SEGMENT_2);
		Delay(SEGMENT_DELAY);
	
		SevenSegLED(temp[3], SEGMENT_3);
	  Delay(SEGMENT_DELAY);
	}
	else //if (temperature >= 100)
	{
		temp[0] = (int)(temperature / div_100);
		temp[1] = (int)((temperature - (temp[0])*div_100)/div);
		temp[2] = (float)temperature - ((temp[0])*div_100 + (temp[1])*div);
		
		SevenSegLED(temp[0], SEGMENT_1);
		Delay(SEGMENT_DELAY);
	
		SevenSegLED(temp[1], SEGMENT_2);
		Delay(SEGMENT_DELAY);
		
		SevenSegLED(temp[2], SEGMENT_3);
	
	}
	
	return;
	
}


/*
 * Function: TURN_ON_OFF_PWM_LED
 * ----------------------------
 * Turns LED on(1) or off(0)
 *
 *   @param: int flip - writesBit on pin on(1) or off(0)
 *   returns: void
 */
void TURN_ON_OFF_PWM_LED(int flip)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_14, flip);
}


/*
 * Function: trigger_alarm
 * ----------------------------
 * Displays single digit number in a segment 
 *
 *   @param: set_alarm_t* alarm - address of struct alarm 
 *
 *   returns: void
 */
void trigger_alarm(set_alarm_t* alarm)
{
	
	// decr -> 1 higher intensity to lower intensity
			if (alarm->decr_Flag)
			{
				while(alarm->count)
				{
					TURN_ON_OFF_PWM_LED(ALARM_ON);
					alarm->count--;
				}
				TURN_ON_OFF_PWM_LED(ALARM_OFF);
				Delay(alarm->Period - alarm->DutyCycle);
				alarm->dutyCycleAdjust -= alarm->width;
				alarm->count = alarm->dutyCycleAdjust;
				
				if (alarm->dutyCycleAdjust <= 0)
				{
					alarm->decr_Flag=0;
					alarm->incr_Flag=1;
					alarm->dutyCycleAdjust = alarm->width;
				}
			}
			
			// incr -> 1 lower intensity to higher intensity
			if (alarm->incr_Flag)
			{
				while(alarm->count <= alarm->dutyCycleAdjust)
				{
					TURN_ON_OFF_PWM_LED(ALARM_ON);
					alarm->count++;
				}
				TURN_ON_OFF_PWM_LED(ALARM_OFF);
				Delay(alarm->DutyCycle-alarm->dutyCycleAdjust);
				alarm->dutyCycleAdjust += alarm->width;
				alarm->count = 0;
				
				if (alarm->dutyCycleAdjust >= alarm->DutyCycle)
				{
					alarm->decr_Flag=1;
					alarm->incr_Flag=0;
					alarm->dutyCycleAdjust = alarm->DutyCycle;
					alarm->count = alarm->DutyCycle;
				}
			}
			
			return;
}

/*
 * Function: DisplayAngle
 * ----------------------------
 * Breaks the floating point angle into 2 segments
 *
 *   @param: float angle - tilt angle in float
 *   @param: uint8_t segment to display
 *   returns: void
 */
void DisplayAngle(float angle, uint8_t SegToDisplay)
{
	
	//init the array
	int32_t temp[3] = {0, 0, 0};
	int div = 10;
	int32_t temp_cast = (int32_t)angle;
	
	if (temp_cast < 0)
	{
		temp[0] = 11;  // Neg
		temp_cast = temp_cast*(-1);
	}
	else
	{
		temp[0] = 12;  // Pos
	}
	
	
	if (angle < 100)
	{
		temp[1] = temp_cast / div;
		temp[2] = temp_cast - (temp[1])*div;
	}
	else 
	{
		return;
	}
	
	if (SegToDisplay == 1)
	{
		SevenSegLED(temp[0], SEGMENT_1);
	}
	else if (SegToDisplay == 2)
	{
		SevenSegLED(temp[1], SEGMENT_2);
	}
	else if (SegToDisplay == 3)
	{
		SevenSegLED(temp[2], SEGMENT_3);
	}
	
	return;
}
