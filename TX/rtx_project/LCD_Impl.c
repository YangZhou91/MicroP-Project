#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "LCD_Impl.h"

//int GLB_Temperature_display_flag = 1;

/*
 * Function: LCD_GPIO_Config
 * ----------------------------
 * Initializes the GPIO config for the LCD
 *
 *   @param: void
 *   returns: void
 */
void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure_D;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure_D);
	GPIO_InitStructure_D.GPIO_Pin = GPIO_Pin_0 |  GPIO_Pin_4  | GPIO_Pin_5 ;
	GPIO_InitStructure_D.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure_D.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure_D.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_D.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure_D);
	
	GPIO_InitTypeDef GPIO_InitStructure_E;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure_E);
	GPIO_InitStructure_E.GPIO_Pin = GPIO_Pin_2 |  GPIO_Pin_4  | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure_E.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure_E.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure_E.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure_E.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure_E);
	
	// sets the enable bit of the LCD to one
	GPIO_WriteBit (GPIOE, GPIO_Pin_8, Bit_SET);
}

/*
 * Function: LCD_Init
 * ----------------------------
 * Initializes the LCD
 *
 *   @param: void
 *   returns: void
 */
void LCD_Init(void)
{
		LCD_GPIO_Config();
	  LCD_FuncSet();
	  LCD_DisplayOnOff();
	  LCD_clear_display();

	  LCD_Delay(50000);
	
	  LCD_display_CursorHome(); //--> once after initialize to point to position 01
	  LCD_CharacterEntryMode(); //--> increments cursor
}


/*
 * Function: LCD_clear_display
 * ----------------------------
 * Clears the LCD screen
 *
 *   @param: void
 *   returns: void
 */
void LCD_clear_display(void)
{
	    LCD_SelectRegs(LCD_REG_SEL_WRITE_COMMAND);
		  LCD_Command_dataRegs(LCD_CLEAR_DISPLAY);

			LCD_Pulse_Longer(LCD_WAIT_CLEAR_DISPLAY);
}

/*
 * Function: LCD_display_CursorHome
 * ----------------------------
 * Cursor home
 *
 *   @param: void
 *   returns: void
 */
void LCD_display_CursorHome(void)
{
	
	    LCD_SelectRegs(LCD_REG_SEL_WRITE_COMMAND);
		  LCD_Command_dataRegs(LCD_CURSOR_HOME);
	
			LCD_Pulse(50000);
}

/*
 * Function: LCD_CharacterEntryMode
 * ----------------------------
 * Sets character Entry mode
 *
 *   @param: void
 *   returns: void
 */
void LCD_CharacterEntryMode(void)
{
	
	    LCD_SelectRegs(LCD_REG_SEL_WRITE_COMMAND);
		  LCD_Command_dataRegs(LCD_CHAR_ENTRY_MODE);
	
	    LCD_Pulse(50000);
}

/*
 * Function: LCD_FuncSet
 * ----------------------------
 * Sets Function Set
 *
 *   @param: void
 *   returns: void
 */
void LCD_FuncSet(void)
{
	    LCD_SelectRegs(LCD_REG_SEL_WRITE_COMMAND);
		  LCD_Command_dataRegs(LCD_FUNCTION_SET);
	
      LCD_Pulse(50000);
}

/*
 * Function: LCD_DisplayOnOff
 * ----------------------------
 * Turns LCD Display On/Off
 *
 *   @param: void
 *   returns: void
 */
void LCD_DisplayOnOff(void)
{
	    LCD_SelectRegs(LCD_REG_SEL_WRITE_COMMAND);
		  LCD_Command_dataRegs(LCD_DISPLAY_ON_OFF);
	
      LCD_Pulse_Longer(1000);

}

/*
 * Function: LCD_write
 * ----------------------------
 * Writes text to the screen 
 * by setting data bits from num.
 *
 *   @param: num -> to write in the data regs
 *   returns: void
 */
void LCD_write(uint8_t num)
{
	LCD_SelectRegs(LCD_REG_SEL_WRITE_TEXT);    
	LCD_Command_dataRegs(num);

  LCD_Pulse(50000);

}

/*
 * Function: LCD_set
 * ----------------------------
 * Sets cursor to the position
 * set by pos.
 *
 *   @param: pos -> position in the display
 *   returns: void
 */
void LCD_set(uint8_t pos)
{
	LCD_SelectRegs(LCD_REG_SEL_WRITE_COMMAND);		
	LCD_Command_dataRegs(pos);
		
  LCD_Pulse(50000);
}


/*
 * Function: LCD_Command_dataRegs
 * ----------------------------
 * Writes "data" to the data regs
 * to send command
 *
 *   @param: data -> to write in the data regs
 *   returns: void
 */
void LCD_Command_dataRegs(uint8_t data)
{
	int dataReg[LCD_DATA_REG_SIZE];
	uint8_t mask = 1;
	int i=0;
	
	//breaks data into 8 bits
	while (i < LCD_DATA_REG_SIZE)
	{
		dataReg[i] = data & mask;
		mask = mask << 1;
		i++;
	}
	
	// sets data reg bits accordingly
	GPIO_WriteBit (GPIOD, GPIO_Pin_0, dataReg[0]);
	GPIO_WriteBit (GPIOE, GPIO_Pin_2, dataReg[1]);
	GPIO_WriteBit (GPIOE, GPIO_Pin_4, dataReg[2]);

	GPIO_WriteBit (GPIOE, GPIO_Pin_5, dataReg[3]);
	GPIO_WriteBit (GPIOD, GPIO_Pin_4, dataReg[4]);	
	GPIO_WriteBit (GPIOD, GPIO_Pin_5, dataReg[5]);

	GPIO_WriteBit (GPIOE, GPIO_Pin_6, dataReg[6]);	
	GPIO_WriteBit (GPIOE, GPIO_Pin_7, dataReg[7]);
	
	return;
	
}


/*
 * Function: LCD_SelectRegs
 * ----------------------------
 * write text for RegSel_RW --> 1
 * write command for RegSel_RW --> 0
 *
 *   @param: int
 *   returns: void
 */
void LCD_SelectRegs(int RegSel_RW)
{
	if (RegSel_RW == LCD_REG_SEL_WRITE_COMMAND)
	{
			GPIO_WriteBit (GPIOE, GPIO_Pin_9, Bit_RESET);	
		  GPIO_WriteBit (GPIOE, GPIO_Pin_10, Bit_RESET);
	}
	else
	{
			GPIO_WriteBit (GPIOE, GPIO_Pin_9, Bit_RESET);	
		  GPIO_WriteBit (GPIOE, GPIO_Pin_10, Bit_SET);
	}
	return;
	
}

/*
 * Function: LCD_Delay
 * ----------------------------
 * Delay counter
 *
 *   @param: uint32_t
 *   returns: void
 */
void LCD_Delay (uint32_t count)
{
	while (count)
	{
		count--;
	}
}


/*
 * Function: LCD_Delay_Longer
 * ----------------------------
 * Dlonger delay counter
 *
 *   @param: uint32_t
 *   returns: void
 */
void LCD_Delay_Longer (uint32_t count)
{
	while (count)
	{
		LCD_Delay(50000);
		count--;
	}
}

/*
 * Function: LCD_Pulse_Longer
 * ----------------------------
 * Produces longer Enable pulse 
 *
 *   @param: uint32_t
 *   returns: void
 */
void LCD_Pulse_Longer (uint32_t delay_longer)
{
		GPIO_WriteBit (GPIOE, GPIO_Pin_8, Bit_RESET);
		LCD_Delay_Longer(delay_longer);
		GPIO_WriteBit (GPIOE, GPIO_Pin_8, Bit_SET);
}


/*
 * Function: LCD_Pulse
 * ----------------------------
 * Produces shorter Enable pulse 
 *
 *   @param: uint32_t
 *   returns: void
 */
void LCD_Pulse (uint32_t delay)
{
		GPIO_WriteBit (GPIOE, GPIO_Pin_8, Bit_RESET);
		LCD_Delay(delay);
		GPIO_WriteBit (GPIOE, GPIO_Pin_8, Bit_SET);
}


/*
 * Function: LCD_Display_String
 * ----------------------------
 * Display the string sentence 
 *
 *   @param_1: char* string
 *   @param_2: int string size
 *   returns: void
 */
void LCD_Display_String(char* sentence, int sentenceSize)
{
	uint8_t i=0;
	
	//clear the display
	LCD_clear_display();
	
	// Check first line size
	if (sentenceSize >= LCD_DISPLAY_FIRST_LINE_SIZE)
	{
		return;
	}
	
	//write the string
	while(i < sentenceSize)
	{
		LCD_write(sentence[i]);
		i++;
	}
	return;
	
}


/*
 * Function: LCD_DISPLAY_UPDATE_POS
 * ----------------------------
 * Display the string sentence 
 * on the position pos without clearing.
 *
 *   @param_1: uint8_t pos - position
 *   @param_2: char* string
 *   returns: void
 */
void LCD_DISPLAY_UPDATE_POS(uint8_t pos, char* sentence)
{
	LCD_set(pos);
	
	while(*sentence != '\0')
	{
		LCD_write(*sentence);
		sentence++;
	}
	return;
	
}

