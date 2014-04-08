#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "cmsis_os.h"
#include "keypad.h"
void keypad_column_init(){
	// initialize GPIO pins
	GPIO_InitTypeDef  GPIO_InitStructure_col;
	NVIC_InitTypeDef   NVIC_InitStructure_keypad_col;
	EXTI_InitTypeDef   EXTI_InitStructure_keypad_col;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* Pin assignment from left to right 
	col 1 --> PD1
	col 2 --> PD2
	col 3 --> PD3
	col 4 --> PD6
	row 1 --> PD7
	row 2 --> PD8
	row 3 --> PD9
	row 4 --> PD10
	*/
  
  GPIO_InitStructure_col.GPIO_Pin = GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10;
  GPIO_InitStructure_col.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure_col.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure_col.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure_col.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure_col);
	
	GPIO_InitStructure_col.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6;
  GPIO_InitStructure_col.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure_col.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure_col.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure_col.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOD, &GPIO_InitStructure_col);
	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* set the pin 8 */
	GPIO_SetBits(GPIOD, GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10);

	/* Connect EXTI Line3 to PD10 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource10|EXTI_PinSource7|EXTI_PinSource8|EXTI_PinSource9);
	/* Configure EXTI Line3 */
  EXTI_InitStructure_keypad_col.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure_keypad_col.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure_keypad_col.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure_keypad_col.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure_keypad_col);

  /* Enable and set EXTI Line3 Interrupt to the lowest priority */
  NVIC_InitStructure_keypad_col.NVIC_IRQChannel = EXTI3_IRQn; 
  NVIC_InitStructure_keypad_col.NVIC_IRQChannelPreemptionPriority = 0x02; 
  NVIC_InitStructure_keypad_col.NVIC_IRQChannelSubPriority = 0x03;
  NVIC_InitStructure_keypad_col.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure_keypad_col);
}

void keypad_row_init(){
	// initialize GPIO pins
	GPIO_InitTypeDef  GPIO_InitStructure_col;
	NVIC_InitTypeDef   NVIC_InitStructure_keypad_col;
	EXTI_InitTypeDef   EXTI_InitStructure_keypad_col;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* Pin assignment from left to right 
	col 1 --> PD1
	col 2 --> PD2
	col 3 --> PD3
	col 4 --> PD6
	row 1 --> PD7
	row 2 --> PD8
	row 3 --> PD9
	row 4 --> PD10
	*/
  
  GPIO_InitStructure_col.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6;
  GPIO_InitStructure_col.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure_col.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure_col.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure_col.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure_col);
	
	GPIO_InitStructure_col.GPIO_Pin = GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10;
  GPIO_InitStructure_col.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure_col.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure_col.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure_col.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOD, &GPIO_InitStructure_col);
	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* set the pin 8 */
	GPIO_SetBits(GPIOD, GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6);

	/* Connect EXTI Line3 to PD10 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1|EXTI_PinSource2|EXTI_PinSource3|EXTI_PinSource6);
	/* Configure EXTI Line3 */
  EXTI_InitStructure_keypad_col.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure_keypad_col.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure_keypad_col.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure_keypad_col.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure_keypad_col);

  /* Enable and set EXTI Line3 Interrupt to the lowest priority */
  NVIC_InitStructure_keypad_col.NVIC_IRQChannel = EXTI3_IRQn; 
  NVIC_InitStructure_keypad_col.NVIC_IRQChannelPreemptionPriority = 0x02; 
  NVIC_InitStructure_keypad_col.NVIC_IRQChannelSubPriority = 0x03;
  NVIC_InitStructure_keypad_col.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure_keypad_col);
}


int readKeyColumn(){
	uint8_t column_1 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	uint8_t column_2 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
	uint8_t column_3 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);
	uint8_t column_4 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6);
	
	if (column_1)
		return 1;
	else if(column_2)
		return 2;
	else if (column_3)
		return 3;
	else if (column_4)
		return 4;	
	else
		return -1;
}

int readKeyRow(){
	uint8_t row_1 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7);
	uint8_t row_2 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8);
	uint8_t row_3 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);
	uint8_t row_4 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10);
	
	if (row_1)
		return 1;
	else if(row_2)
		return 2;
	else if (row_3)
		return 3;
	else if (row_4)
		return 4;	
	else
		return -1;
}

int read_keypad(int col, int row){
	if (col == 1){
		if (row == 1) return 1;
		if (row == 2) return 4;
		if (row == 3) return 7;
	}else if (col == 2){
		if (row == 1) return 2;
		if (row == 2) return 5;
		if (row == 3) return 8;
		if (row == 4) return 0;
	}else if (col == 3){
		if (row == 1) return 3;
		if (row == 2) return 6;
		if (row == 3) return 9;
	}else 
		return -1;
}

char read_symble(int col, int row){
//	if (col == 1){
//		if (row == 4)
//		return '<';
//	}
//	else if (col == 3){
//		if (row == 4)
//		return '>';
//	}
	 if (col == 4){
		if (row == 1)
			return 'A';
		else if (row == 2)
			return 'B';
		else if (row == 3)
			return 'C';
		else 
			return '\n';
	}
	else 
	return 'v';
}


