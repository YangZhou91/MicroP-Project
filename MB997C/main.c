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
/*!
 @brief Thread to perform menial tasks such as switching LEDs
 @param argument Unused
 */
void thread(void const * argument);

int roll_dutyCycle, pitch_dutyCycle;

//! Thread structure for above thread
osThreadDef(thread, osPriorityNormal, 1, 0);

/*!
 @brief Program entry point
 */
int main (void) {
	
	roll_TIM3_init();
	pitch_TIM4_init();

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

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
	uint8_t testBuffer[4];
	int8_t roll, pitch;
	while(1)
	{	
		wirelessReceive_RX(testBuffer);
		
			//sign of roll
			if (testBuffer[0] > 90)
			{
				roll = (testBuffer[0] - 90)*(-1);
			}
			else
				roll = (int8_t)testBuffer[0];
			
			//sign of pitch
			if (testBuffer[1] > 90)
				pitch = (testBuffer[1] - 90)*(-1);
			else
				pitch = (int8_t)testBuffer[1];
			
			printf("Roll: %d\n", roll);
			printf("Pitch: %d\n", pitch);
			
			/*mapping the roll angle onto motor duty cycles*/
			roll_dutyCycle = (int)(roll * 1.116f) + 1550;
			pitch_dutyCycle = (int)(pitch * 1.086f) + 1500;
			
			/*trigger motor*/
			roll_motor(roll_dutyCycle); 
			pitch_motor(pitch_dutyCycle);
		
	}
	
}

void thread (void const *argument) {
	while(1){
		osDelay(1000);
		GPIOD->BSRRL = GPIO_Pin_12;
		osDelay(1000);
		GPIOD->BSRRH = GPIO_Pin_12;
	}
}
