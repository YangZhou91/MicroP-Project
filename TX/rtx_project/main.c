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

int32_t key_signal = 1;
int8_t motor_init[2] = {0,0};
int8_t mode_A[2] = {65,65};
int8_t mode_B[2] = {66,66};
int8_t mode_C[2] = {67,67};

char mode;
char sequence[2];




void keypad(void const * argument);

osThreadDef(tilt_detection_thread, osPriorityNormal, 1, 0);        // tilt angles detection thread
osThreadDef(tilt_DispFlag_thread, osPriorityNormal, 1, 0);         // tilt angles display thread
osThreadDef(transmitTiltAngles, osPriorityNormal, 1, 0);           // tilt angles transmit thread
//osThreadDef(transmitSequence, osPriorityNormal, 1, 0);
osThreadDef(keypad, osPriorityNormal, 1, 0);

osThreadId tilt_thread;
osThreadId tilt_transmit_thread;
//osThreadId transmit_sequence_thread;
osThreadId keypad_thread;



int main (void) {

	LCD_Init();
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
	

	

	
	printf("I love woman with big tit\n");
	
	
	while(1){
		wirelessTransmit_TX(motor_init);
		keypad_column_init();		
		osDelay(100);
		
		/*read column*/
		int8_t key_column = readKeyColumn();
		keypad_row_init();
		
		/*read row*/
		int8_t key_row = readKeyRow();
		
		mode = read_keypad(key_column, key_row);
			
		if (mode != 'e'){
			if (mode == 'A'){
				printf (">>>>>>>>Entering Mode_1 Real Time Tilting<<<<<<<<\n");
				wirelessTransmit_TX(mode_A);
				while(1){
				tilt_thread = osThreadCreate(osThread(tilt_detection_thread),NULL);
				tilt_transmit_thread = osThreadCreate(osThread(transmitTiltAngles),NULL);
				}
			
				
				
				uint8_t readreg=read_Status_Register(WIRELESS_STATUS_VERSION);
			}else if (mode == 'B'){
				printf (">>>>>>>>Entering Mode_2 Movement Sequence Input<<<<<<<<\n");
				wirelessTransmit_TX(mode_B);
//				printf ("A: positive sign(must be entered). B: negative sign. C: terminate input\n");
//				printf ("IMPORTANT: To input <30,-60,10> <-45,65,8>...<20,-30,5> You shall enter the key as following:\n");
//				printf ("A30B60A10B45A65A08...A20B30A05C\n");
				
				keypad_thread = osThreadCreate(osThread(keypad), NULL);
				
				
				
				
				
			}else if (mode == 'C'){
				printf (">>>>>>>>Entering Mode_3 Record and Play Option<<<<<<<<\n");
				wirelessTransmit_TX(mode_C);
				
			}
		}
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
			osDelay(300);
			
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
					sendSequence(sequence);
					//transmit_sequence_thread = osThreadCreate(osThread(transmitSequence), NULL);
				
					
					
					
				//}
				
				
			}
		}
		
//		
//		
		
		
}





