/*
 * Master_4bits.h
 *
 *  Created on: 12 de mai de 2018
 *      Author: Gabriel
 */

#ifndef MASTER_4BITS_H_
#define MASTER_4BITS_H_


#endif /* MASTER_4BITS_H_ */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"
#include "gpio.h"


/* USER CODE BEGIN Prototypes */
void Convert_UserCMD_To_Int(char *Str, int *RW, int *Data, int *Address);
void Write_Data(int Data, int bits, int Write_Permission);
void Read_Data(int *DataRead, int Read_Permission);
void Write_Address(int Data, int bits);
void ChipSelect(int CS);
void DATA_INPUT_MODE(void);
void DATA_OUTPUT_MODE(void);

