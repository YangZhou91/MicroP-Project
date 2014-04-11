  /*******************************************************************
  * @file    MovingAverageFilter.c
  * @author  Tashreef Anowar, Yang Zhou
  * @date    17-February-2014
  * @brief   This file provides functions to Create 
	*          a Moving Average Filter
	********************************************************************/

#include <stdio.h>
#include <MovingAverageFilter.h>

/*
 * Function: MovingAverageFilter
 * ----------------------------
 * Computes the filtered output
 *
 *   @param: InternalStateFilter_t* - pointer to filter structure
 *   @param: int32_t - unfiltered Vadc
 *
 *   returns: int32_t filtered value
 */

int32_t MovingAverageFilter(InternalStateFilter_t* filterInternalState, int32_t data)
{
	
	int32_t average = 0, sign=0;
	int32_t movingSum;
	int i;
	
	//puts data in the buffer
	filterInternalState->filterBuffer[filterInternalState->newIndex] = data;
	
	// increments the newIndex, if newIndex points to the last 
	// element of the buffer then newIndex is made 0 to point to
	// beginning of the buffer again
	if(filterInternalState->newIndex ==  FILTER_DEPTH-1)
	{
		filterInternalState->newIndex = 0;
	}
	else
	{
		filterInternalState->newIndex++;
	}
	
	// If filter empty then divide sum by newIndex+1
	if (filterInternalState->fullBuffer_flag < FILTER_DEPTH)
	{
		movingSum = 0;
		i=0;
		while(i <= filterInternalState->fullBuffer_flag)
		{
			movingSum = movingSum + (filterInternalState->filterBuffer[i]);
			i++;
		}
		
		average = (movingSum / (filterInternalState->fullBuffer_flag+1));
		filterInternalState->fullBuffer_flag++;
		
	}
	else // if filter full then divide sum by filter size
	{
		movingSum = 0;
		i=0;
		while(i < FILTER_DEPTH)
		{
			movingSum = movingSum + (filterInternalState->filterBuffer[i]);
			i++;
		}
		average = (movingSum / FILTER_DEPTH);
	}
	
		return average;
}


/*
 * Function: MovingAverageFilterTemperature
 * ----------------------------
 * Computes the filtered output
 *
 *   @param: InternalStateFilterTemperature_t* - pointer to filter structure
 *   @param: uint16_t - unfiltered Vadc
 *
 *   returns: float filtered value
 */
float MovingAverageFilterTemperature(InternalStateFilterTemperature_t* filterInternalState, uint16_t VADC)
{
	
	float average = 0;
	uint32_t movingSum;
	int i;
	
	//puts Vadc in the buffer
	filterInternalState->filterBuffer[filterInternalState->newIndex] = VADC;
	
	// increments the newIndex, if newIndex points to the last 
	// element of the buffer then newIndex is made 0 to point to
	// beginning of the buffer again
	if(filterInternalState->newIndex ==  FILTER_DEPTH-1)
	{
		filterInternalState->newIndex = 0;
	}
	else
	{
		filterInternalState->newIndex++;
	}
	
	// If filter empty then divide sum by newIndex+1
	if (filterInternalState->fullBuffer_flag < FILTER_DEPTH)
	{
		movingSum = 0;
		i=0;
		while(i <= filterInternalState->fullBuffer_flag)
		{
			movingSum = movingSum + (uint32_t)(filterInternalState->filterBuffer[i]);
			i++;
		}
		
		average = (float)(movingSum / (uint32_t)(filterInternalState->fullBuffer_flag+1));
		filterInternalState->fullBuffer_flag++;
		
	}
	else // if filter full then divide sum by filter size
	{
		movingSum = 0;
		i=0;
		while(i < FILTER_DEPTH)
		{
			movingSum = movingSum + (uint32_t)(filterInternalState->filterBuffer[i]);
			i++;
		}
		average = (float)(movingSum / FILTER_DEPTH);
	}
	
	return average;
	
}