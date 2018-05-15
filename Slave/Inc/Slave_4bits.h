/*
 * Slave_4bits.h
 *
 *  Created on: 15 de mai de 2018
 *      Author: gabrielfl
 */

#ifndef SLAVE_4BITS_H_
#define SLAVE_4BITS_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "main.h"
#include "gpio.h"

void Read_Address_Request(int *AddrRead);
void Read_Data_of_Address(int AddrRead, int VectorData[]);
void Write_Data_at_Address(int AddrRead, int VectorData[]);
void DATA_INPUT_MODE(void);
void DATA_OUTPUT_MODE(void);



#endif /* SLAVE_4BITS_H_ */
