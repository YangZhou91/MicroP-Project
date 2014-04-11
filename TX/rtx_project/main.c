#include "arm_math.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"
#include "stm32f4_discovery_lis302dl.h"
#include "wireless_spi_config.h"
#include "wireless_Impl.h"
#include "tilt_thread.h"
#include "keypad.h"
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "stm32f4xx_conf.h"
#include "LCD_Impl.h"
#define SIGNAL_MODE_CHANGE 0x01
int32_t key_signal = 1;
int8_t motor_init[2] = {0,0};
int8_t mode_A[2] = {65,65};
int8_t mode_B[2] = {66,66};
int8_t mode_C[2] = {67,67};
int8_t mode_selection = 0;
char mode;
char sequence[2];




void keypad(void const * argument);
void mode_change_thread(void const * argument);
void EXTI_NVIC_Config_PushButton(void);
void PushButton_ISR(void);

osThreadDef(tilt_detection_thread, osPriorityNormal, 1, 0);        // tilt angles detection thread
osThreadDef(tilt_DispFlag_thread, osPriorityNormal, 1, 0);         // tilt angles display thread
osThreadDef(transmitTiltAngles, osPriorityNormal, 1, 0);           // tilt angles transmit thread
//osThreadDef(transmitSequence, osPriorityNormal, 1, 0);
osThreadDef(keypad, osPriorityNormal, 1, 0);
osThreadDef(mode_change_thread, osPriorityNormal,1,0);
osThreadId tilt_thread;
osThreadId tilt_transmit_thread;
//osThreadId transmit_sequence_thread;
osThreadId keypad_thread;
osThreadId tid_mode_change;


int main (void) {

	LCD_Init();
	// Init the pushButton
	EXTI_NVIC_Config_PushButton();
  char testLCD[10] = "Test";
	
	LCD_DISPLAY_UPDATE_POS(0x80, testLCD);
//	int i = ch - '0';
	
/*copy characters*/
//	char *ch = "A12B34A56B15B31B42B12B62B56";
//	int i = 0;
//	char *s = ch;
//	printf ("%s", s);
//	
	
	
	
	/*SIZE OF POINTER*/
	
//	int size = strlen(ch);
//	printf("size: %d", size);
	
	/*help method to extract number from characters*/
//	int8_t r[3];
//	int8_t k = 0;
//	while ( k<3 ){
//		for (int8_t i = 0; i < size ; i++){
//			if(ch[i] == 'A' || ch[i] == 'B'){
//				r[k] = (ch[i+1] - '0')*10 + (ch[i+2] - '0');
//				if (ch[i] == 'B'){
//					r[k] = -r[k];
//				}
//				printf("r[%d] = %d\n", k, r[k]);
//				k++;
//			}
//		}
//	}
//	
	
//	char *copy = "  ";
//	int i;
//	i = 0;
//		while ((*copy = *ch) != ' ') {
//		copy++;
//		ch++;
//		}
	
////	for (int8_t i = 0; i < size ; i++){
//		printf("%c\n", ch[size-1]);
//	//	printf("%s", copy);
//		
////	}
//	
	

//	printf("%d\n", size);
//	printf("%s", ch);



	wireless_spi_Init();
	

	

	char welcome[8] = "Welcome"; 
	LCD_DISPLAY_UPDATE_POS(0x80, welcome);
	printf("I love woman with big tit\n");
	tid_mode_change = osThreadCreate(osThread(mode_change_thread),NULL);
	
	while(1){
		
//		wirelessTransmit_TX(motor_init);
//		keypad_column_init();		
//		osDelay(100);
//		
//		/*read column*/
//		int8_t key_column = readKeyColumn();
//		keypad_row_init();
//		
//		/*read row*/
//		int8_t key_row = readKeyRow();
//		
//		mode = read_keypad(key_column, key_row);
//			
//		if (mode != 'e'){
//			if (mode == 'A'){
//				printf (">>>>>>>>Entering Mode_1 Real Time Tilting<<<<<<<<\n");
//				wirelessTransmit_TX(mode_A);
//				while(1){
//				tilt_thread = osThreadCreate(osThread(tilt_detection_thread),NULL);
//				tilt_transmit_thread = osThreadCreate(osThread(transmitTiltAngles),NULL);
//				}
//			
//				
//				
//				uint8_t readreg=read_Status_Register(WIRELESS_STATUS_VERSION);
//			}else if (mode == 'B'){
//				printf (">>>>>>>>Entering Mode_2 Movement Sequence Input<<<<<<<<\n");
//				wirelessTransmit_TX(mode_B);
////				printf ("A: positive sign(must be entered). B: negative sign. C: terminate input\n");
////				printf ("IMPORTANT: To input <30,-60,10> <-45,65,8>...<20,-30,5> You shall enter the key as following:\n");
////				printf ("A30B60A10B45A65A08...A20B30A05C\n");
//				
//				keypad_thread = osThreadCreate(osThread(keypad), NULL);
//				
//				
//				
//				
//				
//			}else if (mode == 'C'){
//				printf (">>>>>>>>Entering Mode_3 Record and Play Option<<<<<<<<\n");
//				wirelessTransmit_TX(mode_C);
//				
//			}
//		}
	osDelay(osWaitForever);
	}

	  
	


}















void keypad(void const *argument){
	printf("keypad thread entered!\n");		
	int8_t num = -1;
	int8_t k = 0;;
	
	char ch;
	
	
	
//	while(1){
//		keypad_column_init();		
//		osDelay(200);
//		
//		/*read column*/
//		int8_t key_column = readKeyColumn();
//		keypad_row_init();
//		
//		/*read row*/
//		int8_t key_row = readKeyRow();
//		
//		/*read number*/
//		num = read_keypad(key_column, key_row);
	
//		if (num != -1)
//			printf ("%d\n", num);
//		else{
//			ch = read_symble(key_column, key_row);
//			if (ch != 'e'){
//					printf ("%c\n", ch);
//			}
//		
		while (ch != 'D'){
			keypad_column_init();		
			osDelay(250);
			
			/*read column*/
			int8_t key_column = readKeyColumn();
			keypad_row_init();
			
			/*read row*/
			int8_t key_row = readKeyRow();
			
			ch = read_keypad(key_column, key_row);
			
			
		
			
			
			if (ch != 'e' ){	
//				sequence[k] = ch;
//				printf("sequence[%d]: %c\n", k,sequence[k] );
//				k++;
				
				//if (ch == 'D'){
					sequence[0] = ch;
					sequence[1] = 'D';
					printf ("before sendSequence: %s\n", sequence);
					LCD_clear_display();
					LCD_DISPLAY_UPDATE_POS(0x80, sequence);
					sendSequence(sequence);
					//transmit_sequence_thread = osThreadCreate(osThread(transmitSequence), NULL);
					
					
					
					
				//}
				
				
			}
		}
		
//		
//		
	
}


	void mode_change_thread(void const * argument){
		wirelessTransmit_TX(motor_init);
		keypad_column_init();		
		osDelay(100);
		
		/*read column*/
		int8_t key_column = readKeyColumn();
		keypad_row_init();
		
		/*read row*/
		int8_t key_row = readKeyRow();
		
		mode = read_keypad(key_column, key_row);
			
		while(1){
			mode = read_keypad(key_column, key_row);
				while(mode == 'e'){
						wirelessTransmit_TX(motor_init);
						keypad_column_init();		
						osDelay(100);
						
						/*read column*/
						int8_t key_column = readKeyColumn();
						keypad_row_init();
						
						/*read row*/
						int8_t key_row = readKeyRow();
						
						mode = read_keypad(key_column, key_row);
				}
				if (mode == 'A'){
				printf (">>>>>>>>Entering Mode_1 Real Time Tilting<<<<<<<<\n");
				wirelessTransmit_TX(mode_A);
				
				tilt_thread = osThreadCreate(osThread(tilt_detection_thread),NULL);
				tilt_transmit_thread = osThreadCreate(osThread(transmitTiltAngles),NULL);
							
				uint8_t readreg=read_Status_Register(WIRELESS_STATUS_VERSION);
				
					mode_selection = 1;
					
			}else if (mode == 'B'){
				printf (">>>>>>>>Entering Mode_2 Movement Sequence Input<<<<<<<<\n");
				wirelessTransmit_TX(mode_B);
//				printf ("A: positive sign(must be entered). B: negative sign. C: terminate input\n");
//				printf ("IMPORTANT: To input <30,-60,10> <-45,65,8>...<20,-30,5> You shall enter the key as following:\n");
//				printf ("A30B60A10B45A65A08...A20B30A05C\n");
				
				keypad_thread = osThreadCreate(osThread(keypad), NULL);
				
				
				mode_selection = 2;
				
				
			}else if (mode == 'C'){
				printf (">>>>>>>>Entering Mode_3 Record and Play Option<<<<<<<<\n");
				wirelessTransmit_TX(mode_C);
				mode_selection = 3;
				
			}
			
			osSignalWait(SIGNAL_MODE_CHANGE, osWaitForever);
			if(mode_selection == 1 ){
				osThreadTerminate(tilt_transmit_thread);
				osThreadTerminate(tilt_thread);
			}
			else if(mode_selection == 2){
				osThreadTerminate(keypad_thread);
			}
			else{
			
			}
			
			mode_selection=0;
		}
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
	
		osSignalSet(tid_mode_change, SIGNAL_MODE_CHANGE);
    /* Clear the EXTI line 1 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}




