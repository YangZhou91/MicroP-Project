  /*******************************************************************
  * @file    calibration.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    9-March-2014
  * @brief   This file provides functions to manage the following 
  *          functionalities:
  *           -  Calibrates the x y z axes
	********************************************************************/


#include <stdio.h>
#include <stdint.h>

/*
 * Function: get_calibration_data
 * ----------------------------
 * Get Calibrated data
 *   Returns: void
 *
 *   @param: int32_t*
 *   @param: float*
 *   returns: none
 */
void get_calibration_data(int32_t data[3], float* calibrated_data);
