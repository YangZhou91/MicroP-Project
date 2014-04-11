  /*******************************************************************
  * @file    MEM_Interrupt.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    9-March-2014
  * @brief   This file provides functions to manage the following 
  *          functionalities:
  *           - Initialization and Configuration MEM and EXTI and NVIC
	********************************************************************/

/*
 * Function: MEM_Interrupt_Config
 * ----------------------------
 * Configure Interrupt for MEM
 *   Returns: void
 *
 *   @param: none
 *   returns: none
 */
void MEM_Interrupt_Config(void);

/*
 * Function: EXTI_NVIC_Config
 * ----------------------------
 * Configure EXTI and NVIC pin PE0 line0
 *   Returns: void
 *
 *   @param: none
 *   returns: none
 */
void EXTI_NVIC_Config(void);
