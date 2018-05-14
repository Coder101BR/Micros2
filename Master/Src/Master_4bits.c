/*
 * Master_4bits.c
 *
 *  Created on: 12 de mai de 2018
 *      Author: Gabriel
 */
/* Includes ------------------------------------------------------------------*/
#include "Master_4bits.h"
#include <string.h>



void Convert_UserCMD_To_Int(char *Str, int *RW, int *Data, int *Address)
{
    char StrAux[3];
    /* Update *RW */
    if((Str[0] == 'w') | (Str[0] == 'W'))
    {
        /* Set *RW to write state*/
        *RW = 0;

        /* Update Address */
        StrAux[0] = Str[2];
        StrAux[1] = Str[3];
        *Address = atoi(StrAux); /* Convert string to int */

        /* Update Data */
        StrAux[0] = Str[5];
        StrAux[1] = Str[6];
        *Data = atoi(StrAux); /* Convert string to int */

    }

    else if((Str[0] == 'r') | (Str[0] == 'R'))
    {
        /* Set *RW to read state*/
        *RW = 1;

        /* Update Address */
        StrAux[0] = Str[2];
        StrAux[1] = Str[3];
        *Address = atoi(StrAux); /* Convert string to int */
    }

    else
    {
        /* Set *RW to an invalid state*/
      *RW = 55;
    }
}

void Write_Data(int Data, int bits, int Write_Permission)
{
    int WriteData[bits];
    int Mask = 1;
    int i;

    if(Write_Permission == 0)
    {
        for(i =0; i < bits; i++)
        {
            WriteData[i] = (Data & Mask);

            if(WriteData[i] > 0)
            {
                WriteData[i] = GPIO_PIN_SET;
            }
            else
            {
                WriteData[i] = GPIO_PIN_RESET;
            }

            //printf("\nWriteData[%d] = %d", i,WriteData[i]); // Substituir para escrever nos GPIO
            Mask = Mask << 1;
        }

        /* Enable DATA_OUTPUT_MODE */
    	DATA_OUTPUT_MODE();
        /* Write data in data pins*/
        HAL_GPIO_WritePin(GPIOB, Data3_Pin , WriteData[3]);
        HAL_GPIO_WritePin(GPIOB, Data2_Pin , WriteData[2]);
        HAL_GPIO_WritePin(GPIOB, Data1_Pin , WriteData[1]);
        HAL_GPIO_WritePin(GPIOB, Data0_Pin , WriteData[0]);


        // Set 0 to write line and Set 1 to read line
        HAL_GPIO_WritePin(GPIOC, Escrita_Pin , GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, Leitura_Pin , GPIO_PIN_SET);

    }

}

void Read_Data(int *DataRead, int Read_Permission)
{
	int InputArray[4];

    if(Read_Permission == 1)
    {
        // Set 1 to write line and Set 0 to read line
        HAL_GPIO_WritePin(GPIOC, Escrita_Pin , GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, Leitura_Pin , GPIO_PIN_RESET);

        /* Clean variable */
        *DataRead = 0;

        /* Read data from the slave and store it at an array variable */
        InputArray[0] = HAL_GPIO_ReadPin(GPIOB, Data3_Pin);
        InputArray[1] = HAL_GPIO_ReadPin(GPIOB, Data2_Pin);
        InputArray[2] = HAL_GPIO_ReadPin(GPIOB, Data1_Pin);
        InputArray[3] = HAL_GPIO_ReadPin(GPIOB, Data0_Pin);

        *DataRead = ((InputArray[0] << 3) | (InputArray[1] << 2) | (InputArray[2] << 1) | (InputArray[3] << 0));


    }

}

void Write_Address(int Data, int bits)
{
    int WriteData[bits];
    int Mask = 1;
    int i;

    for(i =0; i < bits; i++)
    {
        WriteData[i] = (Data & Mask);

        if(WriteData[i] > 0)
        {
            WriteData[i] = GPIO_PIN_SET;
        }
        else
        {
            WriteData[i] = GPIO_PIN_RESET;
        }

        // printf("\nAddress[%d] = %d", i,WriteData[i]); // Substituir para escrever nos GPIO
        Mask = Mask << 1;
    }

    /* Write Address in Address pins*/
    HAL_GPIO_WritePin(GPIOA, Addr3_Pin , WriteData[3]);
    HAL_GPIO_WritePin(GPIOA, Addr2_Pin , WriteData[2]);
    HAL_GPIO_WritePin(GPIOA, Addr1_Pin , WriteData[1]);
    HAL_GPIO_WritePin(GPIOA, Addr0_Pin , WriteData[0]);

}

void ChipSelect(int CS)
{
    /* Select the device to write/read*/
    if(CS == 0)
    {
        HAL_GPIO_WritePin(GPIOC, Chip_Select_Pin , GPIO_PIN_RESET);
    }
    else
    {
    	HAL_GPIO_WritePin(GPIOC, Chip_Select_Pin , GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, Escrita_Pin , GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, Leitura_Pin , GPIO_PIN_SET);

        DATA_INPUT_MODE();
    }


}

void DATA_INPUT_MODE(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin*/
  GPIO_InitStruct.Pin = Data3_Pin|Data2_Pin|Data1_Pin|Data0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void DATA_OUTPUT_MODE(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Data3_Pin|Data2_Pin|Data1_Pin|Data0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin */
  GPIO_InitStruct.Pin =  Data3_Pin|Data2_Pin|Data1_Pin|Data0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
