/*
 * Utils.h
 *
 *  Created on: 12 de jun de 2018
 *      Author: Gabriel
 */

#ifndef UTILS_H_
#define UTILS_H_



#endif /* UTILS_H_ */


/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "main.h"

int _io_putchar(int ch);
int _write(int file, char *ptr, int len);
void DecimalToBin4bits(int NumbertoConvert, char StrBinary)