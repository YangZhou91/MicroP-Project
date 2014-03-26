#include <stdio.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <accelerometer_conf.h>
#include <stm32f4_discovery_lis302dl.h>
#include "accelerometer_conf.h"
#include "calibration.h"
#include "tilt_angle_calculation.h"
#include "MovingAverageFilter.h"
#include <MEM_Interrupt.h>
#include <SevenSegLED.h>
#include "math.h"
#include "LCD_Impl.h"
#include "tilt_thread.h"
#include "PWM_Servo_Impl.h"
#include "temperature_thread.h"

extern	osThreadId temperature_TurnDispFlag_thread;

void tilt_detection_thread(void const * argument);

static volatile uint_fast16_t Interrupt_Flag, Interrupt_Timer_LED;
static float roll, pitch;
static int reset_flag=1;

static uint8_t DisplayTiltFlag = 1; // Flag gets turned on then in tilt mode
int GLB_TiltAngle_RollPitch_display_flag = 1; // Flag that display the string "Roll" and "Pitch" once every mode change

int32_t raw_data[3] = {0,0,0};

osThreadId tiltGetId, turnTiltFlagGetId;

/*!
 @brief Thread to perform detection of tilt
 @param argument Unused
*/
void tilt_detection_thread(void const *argument) 
{
	
	//contains filtered data
	int32_t filtered_data[3] = {0, 0, 0};
	float calibrated_data[3] = {0,0,0};
	reset_flag = 1;
	roll = 0, pitch = 0;
	
	set_Accelerometer_MEM();
	Delay(100);
	
	MEM_Interrupt_Config();
	Delay(100);
	
	EXTI_NVIC_Config();
	Delay(100);
	
	PWM_Servo_config();
	Delay(100);
	
	tiltGetId = osThreadGetId();
	
	/*
	 * Create new filter struct object for filtering x, y and z and initialize
	 */
	
	// struct for filtering x
	int init_buffer = 0;               // index used to initialize the buffer
	InternalStateFilter_t newFilter_x;
	newFilter_x.newIndex = 0;
	newFilter_x.fullBuffer_flag = 0;
	while(init_buffer < FILTER_DEPTH)
	{
		newFilter_x.filterBuffer[init_buffer] = 0;
		init_buffer++;
	}
	
	// struct for filtering y	
	init_buffer = 0;  
	InternalStateFilter_t newFilter_y;
	newFilter_y.newIndex = 0;
	newFilter_y.fullBuffer_flag = 0;
	while(init_buffer < FILTER_DEPTH)
	{
		newFilter_y.filterBuffer[init_buffer] = 0;
		init_buffer++;
	}
	
	// struct for filtering z	
	init_buffer = 0;  
	InternalStateFilter_t newFilter_z;
	newFilter_z.newIndex = 0;
	newFilter_z.fullBuffer_flag = 0;
	while(init_buffer < FILTER_DEPTH)
	{
		newFilter_z.filterBuffer[init_buffer] = 0;
		init_buffer++;
	}
	
	
	LIS302DL_ReadACC(raw_data);   // read raw data
	while(1)
	{   
    // wait for the signal data ready sent by the ISR in exti1		
		osSignalWait(SIGNAL_TILT_READY, osWaitForever);

		//Filter out of noise from raw data
		filtered_data[0] = MovingAverageFilter(&newFilter_x, raw_data[0]);
		filtered_data[1] = MovingAverageFilter(&newFilter_y, raw_data[1]);
		filtered_data[2] = MovingAverageFilter(&newFilter_z, raw_data[2]);
	
		//Filter out of noise from raw data
		get_calibration_data(filtered_data, calibrated_data);
	
		// Calculate roll and pitch
		roll = calculate_roll(calibrated_data);
		pitch = calculate_pitch(calibrated_data);
	
		// if tilt mode on the call PWM_Display to adjust the CCR for PWM servo 
		if (DisplayTiltFlag == 1)
		{	
		 PWM_Display(roll);
		}
		
	}
}


/*
 * Function: LCD_DisplayTiltAngle
 * ----------------------------
 * Display the tilt in LCD
 * Calls LCD API
 *
 *   @param_1: float roll
 *   @param_2: float pitch
 *   returns: void
 */
void LCD_DisplayTiltAngle(float roll, float pitch)
{
	
	char dispRoll[] = "Roll:";
	char dispPitch[] = "Pitch:";
	char bufferRoll[9];
	char bufferPitch[9];
	
	// converts roll and pitch from float to character string 
	sprintf(bufferRoll,"%f", roll);
	sprintf(bufferPitch,"%f", pitch);
	
	// Flag that display the string "Roll" and "Pitch" once every mode change
	if (GLB_TiltAngle_RollPitch_display_flag)
	{
		LCD_clear_display();
		LCD_Delay_Longer(LCD_WAIT_BETWEEN_DISPLAY);
		
		// displays the string "Roll" and "Pitch" once every mode change in first and second line
	  LCD_DISPLAY_UPDATE_POS(0x80, dispRoll);
		LCD_DISPLAY_UPDATE_POS(0xC0, dispPitch);
	  
		GLB_TiltAngle_RollPitch_display_flag=0;
	}
	
	// wait between every display
	LCD_Delay_Longer(LCD_WAIT_BETWEEN_DISPLAY);
	LCD_DISPLAY_UPDATE_POS(0x85, bufferRoll);
	LCD_DISPLAY_UPDATE_POS(0xC6, bufferPitch);
	
}

/*!
 @brief Thread to display tilt angles
 @param argument Unused
*/
void tilt_DispFlag_thread(void const * argument)
{
	
	// Extracts the semaphore object from the argument.
	// This semaphore object was created in the main
	osSemaphoreId* semaphore;
	semaphore = (osSemaphoreId*)argument;
	
	uint8_t ResourceLocked = 1; // Flag gets turned on if semaphore_lock achieved
	uint32_t tokentilt;         // Return of semwait

	osEvent event;

	while(1)
	{
		tokentilt=osSemaphoreWait (*semaphore, osWaitForever);
    ResourceLocked=1;		// turn flag on if resource_locked
    DisplayTiltFlag = 1; // for PWM servo demo
		
    while (ResourceLocked)
		{
			// wait for a signal with 1ms timeout
			event = osSignalWait(SIGNAL_DISP_TILT, 1);
			if (event.status == osEventTimeout)
			{
				// in case of timeout display
				LCD_DisplayTiltAngle(roll, pitch);
			}
			else
			{
				// signal received which is not timeout
				// clear the signal flag for the temperature display thread
				//osSignalClear (temperature_TurnDispFlag_thread, SIGNAL_DISP_TEMPERATURE);
				
				// turn the resource_locked flag off
				ResourceLocked = 0;
				DisplayTiltFlag = 0;
				
				// turn the flag to display the string "temperature" once
				GLB_TiltAngle_RollPitch_display_flag=1;
				
				// release the semaphore 
				osSemaphoreRelease(*semaphore);
				
				//wait 5ms for the other thread to grab the semaphore
				osDelay(5);
			}
		}
		
	}

}


void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {

		// send a data ready signal to the thread that calculates tilt and wake up the thread
		osSignalSet(tiltGetId, SIGNAL_TILT_READY);
		LIS302DL_ReadACC(raw_data);
    
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

