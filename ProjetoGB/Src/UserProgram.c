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
void CallUserProgram(int Digital_Analog, int Digital_Input, int *Output)
{

	*Output = *Output | 14;

	/* Check input 0 */
	if((Digital_Input & 1) == 0)
	{
		//*Output = 1;
	}

	/* Check all digital inputs */
	if(Digital_Input == 0)
	{
	//	*Output = *Output | 12;
	}

	/* Check analog input */
	if(Digital_Analog == 255)
	{
	//	*Output = *Output | 2;
	}
	else if (Digital_Analog <= 128)
	{
	//	*Output = *Output = 0;
	}

}
