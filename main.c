# include <stdio.h>
# include <stdlib.h>
# include "cmsis_os.h"
# include "tilt_thread.h"
# include "stm32f4xx.h"
# include "keypad.h"


/* Definition of threads */
osThreadDef(tilt_detection_thread, osPriorityNormal, 1, 0); 
osThreadDef(tilt_DispFlag_thread, osPriorityNormal, 1, 0);         // display tilt angles thread


// semaphore object created
osSemaphoreId semaphore; // Semaphore ID
osSemaphoreDef(semaphore);



int main(void){
		
	// Thread ids
	osThreadId tilt_thread;
	osThreadId tilt_TurnDispFlag_thread;
	
	
	// semaphore created with resource 1
	//semaphore = osSemaphoreCreate(osSemaphore(semaphore), 1);

	tilt_TurnDispFlag_thread = osThreadCreate(osThread(tilt_DispFlag_thread), NULL);
	
	tilt_thread = osThreadCreate(osThread(tilt_detection_thread),NULL); // Calculate pitch and row
	osThreadDef(tilt_DispFlag_thread, osPriorityNormal, 1, 0);         // display tilt angles thread
	keypad_init();
	// put the main to sleep
	while(1){
		//osDelay(osWaitForever);
		//if(readKeys() != 0xFF){
		osDelay(1000);
				uint8_t number_pressed;
				number_pressed = readKeys();
				printf("The number  %u \n", number_pressed);
		
		//}
	}
}