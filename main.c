#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "stm32f4_discovery_lis302dl.h"
#include "temperature_thread.h"
#include "tilt_thread.h"
#include "LCD_Impl.h"
#include <SevenSegLED.h>

#define PUSH_BUTTON_SIGNAL 0x01

// semaphore object created
osSemaphoreId semaphore; // Semaphore ID
osSemaphoreDef(semaphore);

void EXTI_NVIC_Config_PushButton(void);
void PushButton_ISR(void);

// array containing the thread ids of tilt and temperature display threads
osThreadId SendSigTothreadID_PushButtonInt[2];

//! Thread def for above thread
osThreadDef(temperature_detection_thread, osPriorityNormal, 1, 0); // temperature detection thread
osThreadDef(tilt_detection_thread, osPriorityNormal, 1, 0);        // tilt angles detection thread

osThreadDef(temperature_DispFlag_thread, osPriorityNormal, 1, 0);  // display temperature thread
osThreadDef(tilt_DispFlag_thread, osPriorityNormal, 1, 0);         // display tilt angles thread
osThreadDef(Alarm_LED_thread, osPriorityNormal, 1, 0);             // alarm implementation thread

// thread ids
osThreadId temperature_TurnDispFlag_thread;
osThreadId tilt_TurnDispFlag_thread;

osThreadId Alarm_LED_threadId;

//osThreadDef(ButtonPress_thread, osPriorityNormal, 1, 0);
/*!
 @brief Program entry point
 */

	
int main (void) {
	
	// ID for thread
	osThreadId temperature_thread;
	osThreadId tilt_thread;

	// semaphore created with resource 1
	semaphore = osSemaphoreCreate(osSemaphore(semaphore), 1);
  
	// Init the pushButton
	EXTI_NVIC_Config_PushButton();
	
	// Init the LCD
  LCD_Init();
	
	// Start threads
  Alarm_LED_threadId = osThreadCreate(osThread(Alarm_LED_thread), NULL);
	
	temperature_thread = osThreadCreate(osThread(temperature_detection_thread), NULL);	tilt_thread = osThreadCreate(osThread(tilt_detection_thread),NULL);
	
	// start the tilt and temperature detection threads and pass the pointer to the semaphore object as argument
	temperature_TurnDispFlag_thread = osThreadCreate(osThread(temperature_DispFlag_thread), &semaphore);
	tilt_TurnDispFlag_thread = osThreadCreate(osThread(tilt_DispFlag_thread), &semaphore);
	
	// Fill in the array containing the thread ids of tilt and temperature display threads
  SendSigTothreadID_PushButtonInt[0] = temperature_TurnDispFlag_thread;
	SendSigTothreadID_PushButtonInt[1] = tilt_TurnDispFlag_thread;
	
	// put the main to sleep
	while(1)
	{
		osDelay(osWaitForever);
	}

}

/*
 * Function: PushButton_ISR
 * ----------------------------
 * Push Button ISR
 *
 *   @param: void
 *   returns: void
 */
void PushButton_ISR(void)
{
	// send signal to both tilt and temperature display threads
	osSignalSet(SendSigTothreadID_PushButtonInt[0], SIGNAL_DISP_TEMPERATURE);
  osSignalSet(SendSigTothreadID_PushButtonInt[1], SIGNAL_DISP_TILT);
	
	return;
}

/*
 * Function: EXTI_NVIC_Config_PushButton
 * ----------------------------
 * Configure EXTI and NVIC pin PA0 line0 for push button interrupts
 *   Returns: void
 *
 *   @param: none
 *   returns: none
 */
void EXTI_NVIC_Config_PushButton(void)
{
	GPIO_InitTypeDef GPIO_MEM_Init_struct;
	EXTI_InitTypeDef EXTI_MEM_struct;
	NVIC_InitTypeDef NVIC_MEM_struct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// Enable SYSCFG clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_StructInit(&GPIO_MEM_Init_struct);
	
	GPIO_MEM_Init_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_MEM_Init_struct.GPIO_Pin = GPIO_Pin_0;
	GPIO_MEM_Init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
	GPIO_Init(GPIOA, &GPIO_MEM_Init_struct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	EXTI_MEM_struct.EXTI_Line = EXTI_Line0;
  EXTI_MEM_struct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_MEM_struct.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_MEM_struct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_MEM_struct);

  /* Enable and set EXTI Line0 Interrupt */ 
  NVIC_MEM_struct.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_MEM_struct.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_MEM_struct.NVIC_IRQChannelSubPriority = 0x02; // set priority to 2, accelerometer data ready interrupts gets priority of 1
  NVIC_MEM_struct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_MEM_struct);
	
	return;

}

// Push button interrupt isr
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		// call the push button interrupt isr
		PushButton_ISR();
		// wait to avoid button debounce
		LCD_Delay_Longer(100);
		
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}
