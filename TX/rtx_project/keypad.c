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
	
	
	/* set the pin 8 */
	GPIO_SetBits(GPIOD, GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10);


}

void keypad_row_init(){
	// initialize GPIO pins
	GPIO_InitTypeDef  GPIO_InitStructure_col;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
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
	
	/* set the pin 8 */
	GPIO_SetBits(GPIOD, GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6);
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

char read_keypad(int8_t col, int8_t row){
	if (col == 1){
		if (row == 1) return '1';
		if (row == 2) return '4';
		if (row == 3) return '7';
		if (row == 4) return '*';
	}else if (col == 2){
		if (row == 1) return '2';
		if (row == 2) return '5';
		if (row == 3) return '8';
		if (row == 4) return '0';
	}else if (col == 3){
		if (row == 1) return '3';
		if (row == 2) return '6';
		if (row == 3) return '9';
		if (row == 4) return '#';
	}else if (col == 4){
		if (row == 1) return 'A';
		if (row == 2) return 'B';
		if (row == 3) return 'C';
		if (row == 4) return 'D';		
	}
		return 'e';
}


