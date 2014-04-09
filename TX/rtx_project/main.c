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




int32_t key_signal      = 3;
void thread_keypad(void const * argument);











//! Thread def for above thread
//osThreadDef(temperature_detection_thread, osPriorityNormal, 1, 0); // temperature detection thread
osThreadDef(tilt_detection_thread, osPriorityNormal, 1, 0);        // tilt angles detection thread
osThreadDef(transmitTiltAngles, osPriorityNormal, 1, 0);             // tilt transmit thread
//osThreadDef(transmitTiltAngles, osPriorityNormal, 1, 0);             // tilt transmit thread

// thread ids
osThreadId temperature_TurnDispFlag_thread;
osThreadId tilt_TurnDispFlag_thread;
osThreadId Alarm_LED_threadId;
osThreadId tilt_transmit_thread;






osThreadDef(thread_keypad, osPriorityNormal, 1, 0);
osThreadId keypad_thread;






/*!
 @brief Program entry point
 */
int main (void) {

// Init the pushButton
	EXTI_NVIC_Config_PushButton();	
	
	osThreadId tilt_thread;
	// ID for thread
	osThreadId tid_thread1;
	GPIO_InitTypeDef  GPIO_InitStructure;


  tilt_thread = osThreadCreate(osThread(tilt_detection_thread),NULL);
//	tilt_transmit_thread = osThreadCreate(osThread(transmitTiltAngles),NULL);
	
	
	
	
	
	
keypad_thread = osThreadCreate(osThread(thread_keypad), NULL);
	
	
	
	
	
	
	
	wireless_spi_Init();
	
	//----------------TX------------------------------
	uint8_t readreg=0x00;
	
	readreg = read_Status_Register(WIRELESS_STATUS_VERSION);
	
}

void thread (void const *argument) {

}






void thread_keypad(void const *argument){
	printf("keypad thread entered!");		
//	osThreadDef(transmitTiltAngles, osPriorityNormal, 1, 0);             // tilt transmit thread
//	osThreadId tilt_transmit_thread;
	
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
					//tilt_transmit_thread = osThreadCreate(osThread(transmitTiltAngles),NULL);
					
				}else if (ch == 'B'){
					printf("Enter mode 2 \n");
				}else if (ch == 'C'){
					printf("");
				}else
					printf ("%c", ch);
			}
			}
	}
}
// Push button interrupt isr

void EXTI0_IRQHandler(void)

{

  if(EXTI_GetITStatus(EXTI_Line0) != RESET)

  {
		// call the push button interrupt isr
		//PushButton_ISR();
		printf("botton pressed. \n");
//	  osThreadTerminate (tilt_transmit_thread);
//		osThreadTerminate (tilt_TurnDispFlag_thread);
		osThreadTerminate(keypad_thread);
		// wait to avoid button debounce
		//LCD_Delay_Longer(100);

		

    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);

  }
}




