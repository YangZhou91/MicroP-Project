  /*******************************************************************
  * @file    ADC_Impl.h
  * @author  Tashreef Anowar, Yang Zhou
  * @date    17-February-2014
  * @brief   This file provides functions to Initialize 
	*          the GPIO for ADC.
	********************************************************************/
	

/*
 * Function: set_ADC_config
 * ----------------------------
 * Configures GPIO and ADC 
 *
 *   @param:void
 *   returns: void
 */
void set_ADC_config(void);

/*
 * Function: get_ADC_temp
 * ----------------------------
 * Starts software conversion on the ADC channel
 *
 *   @param:void
 *   returns: void
 */
uint16_t get_ADC_temp(void);
