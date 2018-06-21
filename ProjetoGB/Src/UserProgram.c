/*
 * UserProgram.c
 *
 *  Created on: 11 de jun de 2018
 *      Author: Gabriel
 */


/* Includes ------------------------------------------------------------------*/
#include "UserProgram.h"

/* User ScanTime selection */
int ScanTimeLimit()
{
	int TimeLimit = 25;


	return TimeLimit;
}

/* User program space */
void CallUserProgram(int Digital_Analog, int Digital_Input, int *Output)
{

	*Output = *Output | 15;

	/* Check input 0 */
	if((Digital_Input & 1) == 0)
	{
		*Output = 1;
	}

	/* Check input 1 */
	if((Digital_Input & 2) == 0)
	{
		*Output = 2;
	}

	/* Check input 2 */
	if((Digital_Input & 4) == 0)
	{
		*Output = 4;
	}

	/* Check input 3 */
	if((Digital_Input & 8) == 0)
	{
		*Output = 8;
	}

	/* Check all digital inputs */
	if(Digital_Input == 0)
	{
	//	*Output = *Output | 12;
	}

	/* Check analog input */
	if(Digital_Analog >= 250)
	{
		*Output = *Output = 0;
	}
	else if ((Digital_Analog >= 110) && (Digital_Analog <= 135))
	{
		*Output = *Output = 9;
	}

}
