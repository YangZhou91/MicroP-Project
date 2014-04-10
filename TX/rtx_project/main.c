#include "arm_math.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "stm32f4_discovery_lis302dl.h"
#include "wireless_spi_config.h"
#include "wireless_Impl.h"
#include "tilt_thread.h"

#include "keypad.h"
#include "stdio.h"
#include <math.h>
#include <inttypes.h>
#include "stm32f4xx_conf.h"
#include "userbutton_config.h"
#include "mode2_thread.h"

int32_t key_signal = 3;
// Function prototype
void thread_keypad(void const * argument);
void mode2_thread(void const *argument);
//void transmitTiltAngles(void const * argument);
static void set_mode2_signal(void);
static void set_mode1_signal(void);

// Thread ids
osThreadId temperature_TurnDispFlag_thread;
osThreadId tilt_TurnDispFlag_thread;
//osThreadId Alarm_LED_threadId;
osThreadId tilt_transmit_thread;
osThreadId keypad_thread;
static osThreadId tid_mode2;
static osThreadId tid_mode1;

// Thread defination
//osThreadDef(temperature_detection_thread, osPriorityNormal, 1, 0); // temperature detection thread
osThreadDef(tilt_detection_thread, osPriorityNormal, 1, 0);        // tilt angles detection thread
osThreadDef(transmitTiltAngles, osPriorityNormal, 1, 0);             // tilt transmit thread
osThreadDef(thread_keypad, osPriorityNormal, 1, 0);
osThreadDef(mode2_thread, osPriorityNormal, 1, 0);



// Semaphore creation
//osSemaphoreId semaphore;
//osSemaphoreDef(semaphore);



// Thread status which will be used in the thread termination
osStatus mode1_thread_status;
osStatus mode2_thread_status;

/*!
 @brief Program entry point
 */
int main (void) {

	//Create semaphore with one resource
//	semaphore = osSemaphoreCreate(osSemaphore(semaphore), 1);
	
// Init the pushButton
	EXTI_NVIC_Config_PushButton();	
	
	osThreadId tilt_thread;
	// ID for thread
	osThreadId tid_thread1;
	GPIO_InitTypeDef  GPIO_InitStructure;

//Make thread active 
  tilt_thread = osThreadCreate(osThread(tilt_detection_thread),NULL);
	//tilt_transmit_thread = osThreadCreate(osThread(transmitTiltAngles), NULL);
	keypad_thread = osThreadCreate(osThread(thread_keypad), NULL);
	

	wireless_spi_Init();

	//----------------TX------------------------------
	uint8_t readreg=0x00;
	readreg = read_Status_Register(WIRELESS_STATUS_VERSION);
	while(1){
		osDelay(osWaitForever);
	}
	
}
static void set_mode1_signal(void){
	int32_t signal;
	printf("Enter the set mode1 signal method");
	tid_mode1 = osThreadCreate(osThread(transmitTiltAngles), NULL);
	if(tid_mode1 == NULL){
		printf("The thread didn't create");
	}
	else{
		signal = osSignalSet(tid_mode1, 0x00000005);
	}
}

static void set_mode2_signal(void){
	int32_t signal;
	
	printf("Enter the set_mode2_signal method.\n");
	tid_mode2 = osThreadCreate(osThread(mode2_thread), NULL);
	if(tid_mode2 == NULL){
		printf("The thread didn't create");
	}
	else{
		signal = osSignalSet(tid_mode2, 0x00000005);
 	}
}


void thread_keypad(void const *argument){
	printf("keypad thread entered! \n");		
	
	int num = -1;
	char ch;
	while(1){
		/*init keypad column*/
		keypad_column_init();		
		//osSignalWait(key_signal, osWaitForever);
		/*button deboncing*/
		osDelay(180);
		/*read column*/
		int key_column = readKeyColumn();
		/*init keypad row*/
		keypad_row_init();
		/*read row*/
		int key_row = readKeyRow();
		/*read number*/
		num = read_keypad(key_column, key_row);
		
		if (num != -1){
			printf ("%d \n", num);
		}
			else{
			/*read characters if it is not a number*/
			ch = read_symble(key_column, key_row);
			if (ch != 'v'){
				if (ch == 'A'){
					printf("Enter mode 1 \n");
					mode2_thread_status = osThreadTerminate(tid_mode2);
					set_mode1_signal();

				}else if (ch == 'B'){
					printf("Enter mode 2 \n");
					mode1_thread_status = osThreadTerminate(tid_mode1);
					set_mode2_signal();
				}else if (ch == 'C'){
					printf("Enter mode 3 \n");
				}else
					printf ("%c", ch);
			}
			}
	}
}
void PushBotton_ISR(void){
	
	return;
}

// Push button interrupt isr

void EXTI0_IRQHandler(void)

{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)

  {
		// call the push button interrupt isr
		//PushButton_ISR();
		printf("botton pressed. \n");
		PushBotton_ISR();
		
		// wait to avoid button debounce
		//LCD_Delay_Longer(100);

    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}



