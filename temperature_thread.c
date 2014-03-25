#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_adc.h"
#include "core_cm4.h"
#include <cmsis_os.h>
#include <ADC_Impl.h>
#include <SevenSegLED.h>
#include "LCD_Impl.h"
#include "temperature_thread.h"
#include "tilt_thread.h"
#include <MovingAverageFilter.h>

float temp;

// Flag that display the string "Temperature" once every mode change
int GLB_Temperature_display_flag = 1;

// thread ids for display_tilt_thread and alarm thread
extern	osThreadId tilt_TurnDispFlag_thread;
extern  osThreadId Alarm_LED_threadId;

/*!
 @brief Thread to perform detection of temperature 
 @param argument Unused
*/

void temperature_detection_thread (void const *argument) {
	
	/*Set configuration to ADC for acquiring Vadc*/
	set_ADC_config();
	
	uint16_t Vadc;
	float Vsense, Vadc_filter, Vdd = 3.0, V25 = 0.76, frequency = 25, threshold = 55;
	
  /*
	 * Create new filter struct object for filtering and initialize
	 */
	int init_buffer = 0;               // init the buffer
	InternalStateFilterTemperature_t newFilter;
	newFilter.newIndex = 0;
	newFilter.fullBuffer_flag = 0;
	while(init_buffer < FILTER_DEPTH)
	{
		newFilter.filterBuffer[init_buffer] = 0;
		init_buffer++;
	}
	
	while(1){

		Vadc = get_ADC_temp();
		
		// filter the Vadc
		Vadc_filter = MovingAverageFilterTemperature(&newFilter, Vadc);
		Vsense = Vdd * (float)Vadc_filter / 4095;
		temp = (Vsense - V25)*(float)(1000/2.5) + (float)25;
		
		//printf ("Temp:%f\n", temp); --> FOR DEBUG PURPOSE
		
		// Check temperature threshold for alarm
			if (temp > threshold)
			{
				// if temperature is above the threshold then signal the alarm thread to wake up
				osSignalSet(Alarm_LED_threadId, SIGNAL_ALARM_ON_OFF);
			}
			else 
			{
				// if temperature is below the threshold then make duty cycly 0 
				TIM4->CCR3 =0;
			}

	}
}


/*
 * Function: LCD_DisplayTemperature
 * ----------------------------
 * Display the temperature in LCD
 * Calls LCD API
 *
 *   @param_1: float temperature
 *   returns: void
 */
void LCD_DisplayTemperature(float temperature)
{
	//init the array
	int temp[4] = {0, 0, 0, 0};
	int div = 10;
	float ten=10.0;
	int temp_cast = 0;
	int buffer_size = 0, buffer_size_2 = 0;
	
  // buffers for diaplaying the string "Temperature" and the float temperature
	char buffer_temperature[LCD_DISPLAY_FIRST_LINE_SIZE];
	char buffer_temperatureDigitsSize[LCD_ONLY_TEMPERATURE_SIZE_IN_DIGITS];
	
	// reject less than zero temperature 
	if (temperature < 0)
	{
		printf("Tmeperature:%f is less then zero.\n", temperature);
		return;
	}
	
	// breaks down the floating point temperature into 4 characters
	if (temperature < 100)
	{
		temp[0] = (int)(temperature / div);
		temp[1] = (int)(temperature - (temp[0])*div);
		temp[2] = 10; // dot not required
		temp_cast = (int)temperature;
		temp[3] = (temperature - (float)temp_cast)*ten;
	
	}
	else
	{
		return;
	}
	
  // Displays the string "Temperature" once every mode change
	if (GLB_Temperature_display_flag)
	{
		buffer_size = sprintf (buffer_temperature,"Temperature:");
		
		LCD_Delay_Longer(LCD_WAIT_BETWEEN_DISPLAY);
	  LCD_Display_String(buffer_temperature, buffer_size);
	  
		GLB_Temperature_display_flag=0;
	}
	
	// Display the 4 character  temperature in position 0x8C
	buffer_size_2 = sprintf (buffer_temperatureDigitsSize,"%d%d.%d", temp[0], temp[1], temp[3]);
	
	// wait between every display
	LCD_Delay_Longer(LCD_WAIT_BETWEEN_DISPLAY);
	LCD_DISPLAY_UPDATE_POS(0x8C, buffer_temperatureDigitsSize);
	
	
	return;
	
}



/*!
 @brief Thread to implement alarm 
 @param argument Unused
*/
void Alarm_LED_thread(void const * argument)
{

	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* TIM config */

  GPIO_InitTypeDef GPIO_InitStructure;


  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


  /* LEDs are on GPIOD */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);

  /* Compute the prescaler value */
  int PresVal = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;


  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 700;
  TIM_TimeBaseStructure.TIM_Prescaler = PresVal;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;


  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


  /* PWM1 Mode configuration: Channel3  TIM4 CH3 for GPIOD PD14*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM4, ENABLE);


  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);

  int intensity = 0;

  osEvent event;
  while(1)  // Do not exit
	{ 

		event=osSignalWait(SIGNAL_ALARM_ON_OFF, osWaitForever);
		
		intensity++;
		TIM4->CCR3 = 400 - (intensity + 0) % 400; // set brightness

		Delay(40000);
	
	}
	
	return;
}


/*!
 @brief Thread to display temperature
 @param argument Unused
*/
void temperature_DispFlag_thread(void const * argument)
{
	// Extracts the semaphore object from the argument.
	// This semaphore object was created in the main
	osSemaphoreId* semaphore;
	semaphore = (osSemaphoreId*)argument;
	
	uint8_t ResourceLocked = 1;  // Flag gets turned on if semaphore_lock achieved
	uint32_t tokentemp;          // Return of semwait
	
	// Wait for the semaphore with 1ms timeout
  osEvent event = osSignalWait(SIGNAL_DISP_TEMPERATURE, 1);

	while(1)
	{
		tokentemp=osSemaphoreWait (*semaphore, osWaitForever);
    ResourceLocked=1; // turn flag on if resource_locked		
		
    while (ResourceLocked)
		{
			// wait for a signal with 1ms timeout
			event = osSignalWait(SIGNAL_DISP_TEMPERATURE, 1);
			if (event.status == osEventTimeout)
			{
				// in case of timeout display
				LCD_DisplayTemperature(temp);
			}
			else
			{
				// signal received which is not timeout
				// clear the signal flag for the tilt display thread
				osSignalClear (tilt_TurnDispFlag_thread, SIGNAL_DISP_TILT);
				
				//clear the screen
				LCD_clear_display();
				
				// turn the resource_locked flag off
				ResourceLocked = 0;
				tokentemp=0;
				
				// turn the flag to display the string "temperature" once
				GLB_Temperature_display_flag=1;
				
				// release the semaphore 
				osSemaphoreRelease(*semaphore);
				
				//wait 5ms for the other thread to grab the semaphore
			  osDelay(5);
			}
		}
		
	}
	
}
