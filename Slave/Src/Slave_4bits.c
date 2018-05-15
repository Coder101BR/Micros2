/*
 * Slave_4bits.c
 *
 *  Created on: 15 de mai de 2018
 *      Author: gabrielfl
 */

/* Includes ------------------------------------------------------------------*/
#include "Slave_4bits.h"
#include "gpio.h"

void Read_Address_Request(int *AddrRead)
{
    /* Clean variable */
    *AddrRead = 0;

    /* Read data from HW and put it in a array */
    int ArraySimulation[4] ={0,1,1,0};


    *AddrRead = ((ArraySimulation[0] << 3) | (ArraySimulation[1] << 2) | (ArraySimulation[2] << 1) | (ArraySimulation[3] << 0));

}

void Read_Data_of_Address(int AddrRead, int VectorData[])
{

    int WriteData[4];
    int Mask = 1;
    int i;

    for(i =0; i < 4; i++)
    {
        WriteData[i] = (VectorData[AddrRead] & Mask);

        if(WriteData[i] > 0)
        {
            WriteData[i] = GPIO_PIN_SET;
        }
        else
        {
            WriteData[i] = GPIO_PIN_RESET;
        }

        printf("\nWriteData[%d] = %d", i,WriteData[i]); // Substituir para escrever nos GPIO
        Mask = Mask << 1;
    }

}

void Write_Data_at_Address(int AddrRead, int VectorData[])
{

    int DataSentFromMaster[4];


	DATA_INPUT_MODE(); /* Set data pins to input mode */

	/* Read data sent from master */
    DataSentFromMaster[0] = HAL_GPIO_ReadPin(GPIOB, Data0_Pin);
    DataSentFromMaster[1] = HAL_GPIO_ReadPin(GPIOB, Data1_Pin);
    DataSentFromMaster[2] = HAL_GPIO_ReadPin(GPIOB, Data2_Pin);
    DataSentFromMaster[3] = HAL_GPIO_ReadPin(GPIOB, Data3_Pin);


    VectorData[AddrRead] = ((DataSentFromMaster[0] << 3) | (DataSentFromMaster[1] << 2) | (DataSentFromMaster[2] << 1) | (DataSentFromMaster[3] << 0));

    //printf("Inside the function: %d", VectorData[AddrRead]);

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
