/*
 * UserProgram.c
 *
 *  Created on: 11 de jun de 2018
 *      Author: Gabriel
 */


/* Includes ------------------------------------------------------------------*/
#include "UserProgram.h"

/* User ScanTime selection between xx value to yy */
int ScanTimeLimit()
{
	int TimeLimit = 5000;


	return TimeLimit;
}

/* User program space */
void CallUserProgram(int Digital_Input, int Digital_Analog, int *Output)
{

	/* Check input 0 */
	if((Digital_Input & 1) == 0)
	{
		*Output = 1;
	}

	/* Check all inputs */
	if(Digital_Input == 0)
	{
		*Output = *Output | 12;
	}

}
