//#include "common.h"
#include "keypad.h"

EXTI_InitTypeDef   EXTI_InitStructure_keypad;


void keypad_init(){
	// initialize GPIO pins
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure_keypad;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* back-side up, from left to right: 
	pin 1 --> PD1
	pin 2 --> PD2
	pin 3 --> PD3
	pin 4 --> PD6
	pin 5 --> PD7
	pin 6 --> PD8
	pin 7 --> PD9
	pin 8 --> PD10
	*/
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* set the pin 8 */
	GPIO_SetBits(GPIOD, GPIO_Pin_10);

	/* Connect EXTI Line3 to PD10 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource10);
	/* Configure EXTI Line3 */
  EXTI_InitStructure_keypad.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure_keypad.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure_keypad.EXTI_Trigger = EXTI_Trigger_Falling; // interrupt will be triggered on rising edge 
  EXTI_InitStructure_keypad.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure_keypad);

  /* Enable and set EXTI Line3 Interrupt to the lowest priority */
  NVIC_InitStructure_keypad.NVIC_IRQChannel = EXTI3_IRQn; // add EXTI0_IRQn to NVIC   
  NVIC_InitStructure_keypad.NVIC_IRQChannelPreemptionPriority = 0x02; // set priority level
  NVIC_InitStructure_keypad.NVIC_IRQChannelSubPriority = 0x03;
  NVIC_InitStructure_keypad.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure_keypad);
}

uint8_t readKeys(){
	uint8_t pin1 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	uint8_t pin2 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
	uint8_t pin3 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);
	uint8_t pin4 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6);
	uint8_t pin5 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7);
	uint8_t pin6 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8);
	uint8_t pin7 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);
	uint8_t pin8 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10);
	pin1 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	
	if (pin3){
		if (pin7) return 1;
		if (pin6) return 4;
		if (pin5) return 7;
		if (pin4) return 10;
	}
	else if(pin2){
		if (pin7) return 2;
		if (pin6) return 5;
		if (pin5) return 8;
		if (pin4) return 0;
	}else if (pin1){
		if (pin7) return 3;
		if (pin6) return 6;
		if (pin5) return 9;
		if (pin4) return 11;
	}

	return 0xFF; // when nothing pressed
	
	
	
}

