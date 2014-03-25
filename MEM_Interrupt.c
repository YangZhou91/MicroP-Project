  /*******************************************************************
  * @file    MEM_Interrupt.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    9-March-2014
  * @brief   This file provides functions to manage the following 
  *          functionalities:
  *           - Initialization and Configuration MEM and EXTI and NVIC
	********************************************************************/
	
#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4_discovery_lis302dl.h"
#include "misc.h"
#include <MEM_Interrupt.h>

/*
 * Function: EXTI_NVIC_Config
 * ----------------------------
 * Configure EXTI and NVIC pin PE0 line0
 *   Returns: void
 *
 *   @param: none
 *   returns: none
 */
void EXTI_NVIC_Config(void)
{
	GPIO_InitTypeDef GPIO_MEM_Init_struct;
	EXTI_InitTypeDef EXTI_MEM_struct;
	NVIC_InitTypeDef NVIC_MEM_struct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	// Enable SYSCFG clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_StructInit(&GPIO_MEM_Init_struct);
	
	GPIO_MEM_Init_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_MEM_Init_struct.GPIO_Pin = GPIO_Pin_1;
	GPIO_MEM_Init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
	GPIO_Init(GPIOE, &GPIO_MEM_Init_struct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
	
	EXTI_MEM_struct.EXTI_Line = EXTI_Line1;
  EXTI_MEM_struct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_MEM_struct.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_MEM_struct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_MEM_struct);

  /* Enable and set EXTI Line1 Interrupt */ 
  NVIC_MEM_struct.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_MEM_struct.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_MEM_struct.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_MEM_struct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_MEM_struct);
	
	return;

}


/*
 * Function: MEM_Interrupt_Config
 * ----------------------------
 * Configure Interrupt for MEM
 *   Returns: void
 *
 *   @param: none
 *   returns: none
 */
void MEM_Interrupt_Config(void)
{
	
  
  
  LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;  
  
    
  /* Set configuration of Internal MEM Interrupt of LIS302DL*/
  LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_NOTLATCHED;
  LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_XYZ_DISABLE;
  LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_CLICKINTERRUPT_XYZ_DISABLE;
	
  LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);
  
  /* Configure Interrupt control register: enable Data Ready interrupt1 */
  uint8_t ctrl = 0;
	ctrl = 0x20; // changed from 0x4
	
	LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1);
	
	return;

}
 
