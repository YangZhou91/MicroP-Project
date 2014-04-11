#include "arm_math.h"
#include "stdio.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "wireless_spi_config.h"
#include "wireless_Impl.h"
#include "roll_motor.h"
#include "pitch_motor.h"
#include "roll_PWM_Timer.h"
#include "pitch_PWM_Timer.h"

int CosineInterpolate(int start_point, int end_point, float time);

char mode;
char sequence[2];
int k = 0;

int size = 0;

int num = 0;

int j = 0;

int set = 0;
int roll_dutyCycle, pitch_dutyCycle;


/*!
 @brief Program entry point
 */
int main (void) {
	
	
//				char *sequence = "*12#34#56*78*90#12#23*45*67#89#12*13#23*43#54*65#76";
//				int size = strlen(sequence);
//				int num = size/3;
//	
//				printf("size: %d, num %d\n", size, num);
//				int r[num];
//				int k = 0;
//				
//				while ( k < num ){
//					
//					for (int i = 0; i < size ; i++){
//						if(sequence[i] == '*'){
//							r[k] = (sequence[i+1] - '0')*10 + (sequence[i+2] - '0');
//							
//						if(sequence[i] == '#'){
//								r[k] = - ((sequence[i+1] - '0')*10 + (sequence[i+2] - '0'));
//						}
//						
//							printf("r[%d] = %d\n", k, r[k]);
//							k++;
//						}
//					}
//					
//					
//				}
//}
	
	TIM3_init();
	TIM4_init();

//	GPIO_InitTypeDef  GPIO_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);

  int i=100, j=50000;
	
	wireless_spi_Init();
	while(i)
	{
		while(j)
		{
			j--;
		}
		i--;
	}
	//----------------TX------------------------------
//	uint8_t readreg=0x00;
//	
//	readreg = read_Status_Register(WIRELESS_STATUS_VERSION);
//	//readreg = read_Status_Register(0x0E);
//	printf("Read from wireless: %x\n", readreg);
//	float roll = 187;
//	float pitch = 98;
//	
//	float test[2] = {roll, pitch};
//	
//	wirelessTransmit_TX(test);
	//----------------TX------------------------------
	
	
	//----------------RX------------------------------
	uint8_t testbuf[4];
	int8_t buf1, buf2;
	
	
	while(1)
	{	
		wirelessReceive_RX(testbuf);
		buf1 = (int8_t)testbuf[0];
		buf2 = (int8_t)testbuf[1];
		char b1 = (char)buf1;
		char b2 = (char)buf2;
		
		if (b1 == 'A'){
			mode = 'A';
		}
		
		if (b1 == 'B'){
			mode = 'B';
		}	
		
		if (mode == 'A'){
			printf("MODE 1: %d %d\n", buf1, buf2);

			/*mapping the roll angle onto motor duty cycles*/
			roll_dutyCycle = 1550 - (int)(buf1 * 11.16f) ;
			pitch_dutyCycle =1500 - (int)(buf2 * 10.86f) ;
			
			/*trigger motor*/
			roll_pwm(roll_dutyCycle); 
			pitch_pwm(pitch_dutyCycle);
		}
		
		
		if (mode == 'B'){
			
			//printf("MODE 2: %d %d\n", buf1, buf2);
			
			if (b2 == 'D'){
				printf ("MODE 2: %c\n",b1);	
				sequence[k] = b1;
				k++;
				
			}
			
			
			if (b1 == 'D'){
			
				printf ("%s\n", sequence);
				
				
				/*help method to extract number from characters*/
				size = strlen(sequence);

				num = size/3;
				
				set = num/3;
				
				printf("size: %d, num %d, set %d\n", size, num,set);
				
				int r[num];	

				while ( j < num ){
					
					for (int i = 0; i < size ; i++){
						
						if(sequence[i] == '*'){
							r[j] = (sequence[i+1] - '0')*10 + (sequence[i+2] - '0');
							printf("r[%d] = %d\n", j, r[j]);
							j++;
						}
						
						if(sequence[i] == '#'){
							r[j] = - ((sequence[i+1] - '0')*10 + (sequence[i+2] - '0'));
							printf("r[%d] = %d\n", j, r[j]);
							j++;
						}						
						
					}
				}
				
				//cosine interpolation
				for (int i = 0; i < num-3; i = i+3){
					//CosineInterpolate (r[i], r[i+1], r[i+2]);
					printf("sets <%d, %d, %d>\n", r[i],r[i+1],r[i+2]);
				}
				
				
				
				
			}
			
			//printf("received sequence: %s\n", rec_sequence);
//			if (buf1 > 34 && buf1 < 68){
//				rec_sequence[k] = b1;
//				printf("buf1 %d %c\n", k, b1);
//				k++;
//			}
//			
//			if (buf2 > 34 && buf2 < 68){
//				rec_sequence[k] = b2;
//				printf("buf2 %d %c\n", k, b2);
//				k++;
//			}
//			
//			if (buf1 == 68 || buf2 == 68){
//				printf("received %s\n", rec_sequence);
//				k = 0;
//			}
			
			
			
		}
		
		
		
//		printf("mode: %c\n", mode);	
//		
		
		
		
		
	
		/*read '*'(42) or #(35) */
		
//		if (buf1 == 42 || buf1 == 35){
//			char a = (char)buf1;
//			char b = (char)buf2;
//			while (a != 'D' && b != 'D'){
//				printf ("%c %c\n",buf1,buf2);
//			}
//			
			
			
//			printf(">>>>>>>>>>>>>>>MODE 2<<<<<<<<<<<<<<<\n");
//			printf("%d, %d\n", buf1, buf2);
//			if (buf1 == 68 || buf2 == 68){
//				printf("\n>>>>>>>>>>>>>>>All sequences have been received<<<<<<<<<<<<<\n");
//				
//				break;
//				// go to mode 2 ....
//				
//			}
			
//		}
//		


}
}




//int CosineInterpolate(int start_point, int end_point, float time)
//{
//	
//	float t; 
//	float t2;
//	int inter_point;
//	for (int i = 0; i <= time; i ++){
//		t = ((float)i)/ time;
//		t2 = (1-cos(t*3.14))/2;
//		inter_point = start_point*(1-t2)+end_point*t2;
//		printf("t: %f", t);
//		printf("  t2: %f", t2);
//		printf ("  point%d: %d\n", i, inter_point);
//		PWM_Motor (inter_point);
//		for (int i = 0; i < 3000000; i++);
//	}
//   return 0;
//  
//}