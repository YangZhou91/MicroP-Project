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
//#include <SevenSegLED.h>
#include "math.h"
#include "LCD_Impl.h"
#include "tilt_thread.h"
//#include "PWM_Servo_Impl.h"
#include "temperature_thread.h"
#include "wireless_spi_config.h"
#include "wireless_Impl.h"
#include "string.h"

//#define SIGNAL_DISP_TEMPERATURE 0x05 // signal sent to temperature thread
int transmitFlag;
int transmittingFlag=0; // 1-> When transmitting data, 0 otherwise

char *message;



void tilt_detection_thread(void const * argument);

static volatile uint_fast16_t Interrupt_Flag, Interrupt_Timer_LED;
static float roll, pitch;
static int reset_flag=1;

static uint8_t DisplayTiltFlag = 1; // Flag gets turned on then in tilt mode
int GLB_TiltAngle_RollPitch_display_flag = 1; // Flag that display the string "Roll" and "Pitch" once every mode change

int32_t raw_data[3] = {0,0,0};

osThreadId tiltGetId, transmitGetID, turnTiltFlagGetId;

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
	
	int transmitRollPitch[2];
	
	set_Accelerometer_MEM();

	
	MEM_Interrupt_Config();

	
	EXTI_NVIC_Config();

	
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
   // transmitFlag = 0;
		//Filter out of noise from raw data
		filtered_data[0] = MovingAverageFilter(&newFilter_x, raw_data[0]);
		filtered_data[1] = MovingAverageFilter(&newFilter_y, raw_data[1]);
		filtered_data[2] = MovingAverageFilter(&newFilter_z, raw_data[2]);
	
		//Filter out of noise from raw data
		get_calibration_data(filtered_data, calibrated_data);
	
		// Calculate roll and pitch
		roll = (int8_t)calculate_roll(calibrated_data);
		pitch = (int8_t)calculate_pitch(calibrated_data);
	
		if (transmittingFlag == 0)
		{
			osSignalSet(transmitGetID, SIGNAL_TRANSMIT);
		}
	
		LIS302DL_ReadACC(raw_data);
	}
}

/*!
 @brief Thread to display tilt angles
 @param argument Unused
*/
void tilt_DispFlag_thread(void const * argument)
{
	//uint8_t transmitRoll, transmitPitch; 
	int transmitRollPitch[2];
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

void transmitTiltAngles(void const * argument)
{
	int8_t transmitRollPitch[2];
	char dispRoll[] = "Roll:";
	char dispPitch[] = "Pitch";
	char bufferRoll[9];
	char bufferPitch[9];
	
	
	LCD_clear_display();
	LCD_DISPLAY_UPDATE_POS(0x80, dispRoll);
	LCD_DISPLAY_UPDATE_POS(0xC0, dispPitch);
	transmitGetID = osThreadGetId();
	
	while(1)
	{
		sprintf(bufferRoll, "%f", roll);
	sprintf(bufferPitch, "%f", pitch);
		osSignalWait(SIGNAL_TRANSMIT, osWaitForever);
		
		transmittingFlag=1;
		
		transmitRollPitch[0] = roll;
		transmitRollPitch[1] = pitch;
		printf("%d ", transmitRollPitch[0]);
		printf("%d\n", transmitRollPitch[1]);
		LCD_Delay_Longer(LCD_WAIT_BETWEEN_DISPLAY);
		LCD_DISPLAY_UPDATE_POS(0x85, bufferRoll);
		LCD_DISPLAY_UPDATE_POS(0xC6, bufferPitch);
		wirelessTransmit_TX(transmitRollPitch);
		
		transmittingFlag=0;
	}
	
	return;
}


void sendSequence(char* seq){
	extern char *message;
	message = seq;
	printf ("sendSequence: %s\n", message);
	
		int size = strlen(message);
	
	printf ("size : %d",size);
	int8_t transmit[2] = {0, 0};
	transmitGetID = osThreadGetId();
//	while(1)
//	{
		//osSignalWait(SIGNAL_TRANSMIT, osWaitForever);
		
		transmittingFlag=1;

		for (int i = 0; i < size; i += 2){
			transmit[0] = (int8_t)message[i];
			transmit[1] = (int8_t)message[i+1];
			printf("transmit>>>> %s\n", transmit);
			wirelessTransmit_TX(transmit);
		}
		transmittingFlag=0;
}






//void transmitSequence(void const * argument){
//	
////	extern char *message;
//	printf("Enter transmitSequence....\n");
//	printf ("message: %s\n", message);
//	
//	
//	int size = strlen(message);
//	
//	printf ("size : %d",size);
//	int8_t transmit[2];
//	transmitGetID = osThreadGetId();
////	while(1)
////	{
//		//osSignalWait(SIGNAL_TRANSMIT, osWaitForever);
//		
//		transmittingFlag=1;

//		for (int i = 0; i < size; i += 2){
//			transmit[0] = (int8_t)message[i];
//			transmit[1] = (int8_t)message[i+1];
//			printf("transmit>>>> %s\n", transmit);
//			wirelessTransmit_TX(transmit);
//		}
//		transmittingFlag=0;
////	}
//	
//	return;
//}







void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {

		// send a data ready signal to the thread that calculates tilt and wake up the thread
		osSignalSet(tiltGetId, SIGNAL_TILT_READY);
    
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

