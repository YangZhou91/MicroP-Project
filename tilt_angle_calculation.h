  /*******************************************************************
  * @file    tilt_angle_calculation.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    9-March-2014
  * @brief   This file provides functions to manage the following 
  *          functionalities:
  *           - calculate toll and pitch
	********************************************************************/
	
#define PI 3.14159265



/*
 * Function: calculate_pitch
 * ----------------------------
 * Calculate Pitch
 *   Returns: float
 *
 *   @param: float*
 *   returns: float
 */
float calculate_pitch(float* calibrated_data);


/*
 * Function: calculate_roll
 * ----------------------------
 * Calculate Roll
 *   Returns: float
 *
 *   @param: float*
 *   returns: float
 */
float calculate_roll(float* calibrated_data);
