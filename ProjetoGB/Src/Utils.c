/*
 * Utils.c
 *
 *  Created on: 11 de jun de 2018
 *      Author: Gabriel
 */


/* Includes ------------------------------------------------------------------*/
#include "Utils.h"

/* These two functions below enable printf use for FREERTOS */
int _io_putchar(int ch){
	uint8_t c[1];
	c[0] = ch & 0x00FF;
	HAL_UART_Transmit(&huart2, &*c, 1, 10);
	return ch;
}

int _write(int file, char *ptr, int len){
	int DataIdx;
	for(DataIdx = 0; DataIdx < len; DataIdx++){
		_io_putchar(*ptr++);
	}
	return len;
}

void DecimalToBin4bits(int NumbertoConvert, char StrBinary)
{
	int mask = 0xf;
    int Array[4];
    char buffer [6];
	  
	/* Start conversion from decimal to binary */  
    Array[0] = NumbertoConvert & 1;
    Array[1] = NumbertoConvert & 2;
    Array[2] = NumbertoConvert & 4;
    Array[3] = NumbertoConvert & 8;
	  
    for(int i = 0; i < 4; i++)
    {
        if(Array[i] > 0)
        {
           Array[i] = 1; 
        }
        else
        {
            Array[i] = 0;
        }
       //  printf("\nArray[%d]: %d", i, Array[i]);
       // Enviar para o display
    }
	/* End conversion from decimal to binary */
	
	/* Convert to String */ 
    sprintf (buffer, "%d%d%d%d", Array[3], Array[2], Array[1], Array[0]);
    printf("\n%s", buffer);
	
}