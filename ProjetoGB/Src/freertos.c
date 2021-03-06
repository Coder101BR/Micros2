/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "stm32f3xx_hal.h"
#include "cmsis_os.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "UserProgram.h"
#include "Utils.h"
#include "user_5110.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId InputReadHandle;
osThreadId RunUserProgramHandle;
osThreadId OutputUpdateHandle;
osThreadId DisplayUpdateHandle;
osThreadId HouseKeepingHandle;
osMutexId InputMutexHandle;
osMutexId OutputMutexHandle;
osMutexId ScanMutexHandle;
osMutexId PrintMutexHandle;
osMutexId SPIMutexHandle;

/* USER CODE BEGIN Variables */
#define NUMBER_OF_TASKS 5

int Digital_Input = 0xF; // Pullup
int Analog_Input = 0;
int Digital_Output = 0;
int Error_Flag = 0;
TickType_t Task_Time[NUMBER_OF_TASKS];
TickType_t ScanTimeGlobal = 0;
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartInputRead(void const * argument);
void StartRunUserProgram(void const * argument);
void StartOutputUpdate(void const * argument);
void StartDisplayUpdate(void const * argument);
void StartHouseKeeping(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of InputMutex */
  osMutexDef(InputMutex);
  InputMutexHandle = osMutexCreate(osMutex(InputMutex));

  /* definition and creation of OutputMutex */
  osMutexDef(OutputMutex);
  OutputMutexHandle = osMutexCreate(osMutex(OutputMutex));

  /* definition and creation of ScanMutex */
  osMutexDef(ScanMutex);
  ScanMutexHandle = osMutexCreate(osMutex(ScanMutex));

  /* definition and creation of PrintMutex */
  osMutexDef(PrintMutex);
  PrintMutexHandle = osMutexCreate(osMutex(PrintMutex));

  /* definition and creation of SPIMutex */
  osMutexDef(SPIMutex);
  SPIMutexHandle = osMutexCreate(osMutex(SPIMutex));


  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of InputRead */
  osThreadDef(InputRead, StartInputRead, osPriorityNormal, 0, 128);
  InputReadHandle = osThreadCreate(osThread(InputRead), NULL);

  /* definition and creation of RunUserProgram */
  osThreadDef(RunUserProgram, StartRunUserProgram, osPriorityNormal, 0, 128);
  RunUserProgramHandle = osThreadCreate(osThread(RunUserProgram), NULL);

  /* definition and creation of OutputUpdate */
  osThreadDef(OutputUpdate, StartOutputUpdate, osPriorityNormal, 0, 128);
  OutputUpdateHandle = osThreadCreate(osThread(OutputUpdate), NULL);

  /* definition and creation of DisplayUpdate */
  osThreadDef(DisplayUpdate, StartDisplayUpdate, osPriorityNormal, 0, 128);
  DisplayUpdateHandle = osThreadCreate(osThread(DisplayUpdate), NULL);

  /* definition and creation of HouseKeeping */
  osThreadDef(HouseKeeping, StartHouseKeeping, osPriorityNormal, 0, 128);
  HouseKeepingHandle = osThreadCreate(osThread(HouseKeeping), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartInputRead function */
void StartInputRead(void const * argument)
{
	int InputArray[4];
	int CurrectReading = 0;
	int PreviousReading = 0;
	int i;
	int count_temp = 0;
	int init;

    TickType_t Initial_Time = 0;
    TickType_t Time_Result;
    Task_Time[0] = 0;

  /* USER CODE BEGIN StartInputRead */
  /* Infinite loop */
  for(;;)
  {
	  Initial_Time = osKernelSysTick();

	  /* SPI MUTEX - Analog read */
	  osMutexWait(SPIMutexHandle, 1000);

	  /* Enable !CS */
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
	  init = 0xE; // CH1 selected;
	  HAL_SPI_Transmit(&hspi1, &init, 1, 1000);

	  /* Receive data from CH1 in init variable */
	  HAL_SPI_Receive(&hspi1, &init, 1, 1000);

	  /* Disable !CS */
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);

	  osMutexRelease(SPIMutexHandle);


	  osMutexWait(InputMutexHandle, 1000);

	  /* Debounce of Digital Inputs */
	  for(i = 0; i <= 40; i++)
	  {
		  PreviousReading = CurrectReading;

		  InputArray[0] = HAL_GPIO_ReadPin(GPIOA, Digital_Input_3_Pin);
		  InputArray[1] = HAL_GPIO_ReadPin(GPIOA, Digital_Input_2_Pin);
		  InputArray[2] = HAL_GPIO_ReadPin(Digital_Input_1_GPIO_Port, Digital_Input_1_Pin);
		  InputArray[3] = HAL_GPIO_ReadPin(Digital_Input_0_GPIO_Port, Digital_Input_0_Pin);

		  CurrectReading = ((InputArray[0] << 3) | (InputArray[1] << 2) | (InputArray[2] << 1) | (InputArray[3] << 0));

		  if(PreviousReading == CurrectReading)
		  {
			  count_temp++;
			  if(count_temp >= 5)
			  {
				  Digital_Input = CurrectReading;
				  count_temp = 0;
				  break;
			  }
		  }
		  else
		  {
			  count_temp = 0;
		  }

	  }

	  /* Copy value from ADC to Analog_Input*/
	  Analog_Input = init;

      osMutexRelease(InputMutexHandle);
      count_temp = 0;


      osDelay(1);
      /* Save task Time_Result at Task_Time vector*/
      osMutexWait(ScanMutexHandle, 1000);
      Time_Result =  osKernelSysTick() - Initial_Time;
      Task_Time[0] = Task_Time[0] + Time_Result;
      // printf("Task_Time[0]: %d\r\n", Task_Time[0]);
	  osMutexRelease(ScanMutexHandle);


	  osDelay(10);
  }
  /* USER CODE END StartInputRead */
}

/* StartRunUserProgram function */
void StartRunUserProgram(void const * argument)
{
  /* USER CODE BEGIN StartRunUserProgram */
	TickType_t Initial_Time = 0;
	TickType_t Time_Result;
	Task_Time[1] = 0;
  /* Infinite loop */
  for(;;)
  {
	Initial_Time = osKernelSysTick();

	osMutexWait(InputMutexHandle, 1000);
	osMutexWait(OutputMutexHandle, 1000);

	CallUserProgram(Analog_Input, Digital_Input, &Digital_Output);

	osMutexRelease(OutputMutexHandle);
	osMutexRelease(InputMutexHandle);


	osDelay(1);
    /* Save task Time_Result at Task_Time vector*/
    osMutexWait(ScanMutexHandle, 1000);
    Time_Result =  osKernelSysTick() - Initial_Time;
    Task_Time[1] = Task_Time[1] + Time_Result;
    // printf("Task_Time[1]: %d\r\n", Task_Time[1]);
	osMutexRelease(ScanMutexHandle);

	osDelay(10);
  }
  /* USER CODE END StartRunUserProgram */
}

/* StartOutputUpdate function */
void StartOutputUpdate(void const * argument)
{
  /* USER CODE BEGIN StartOutputUpdate */
    int WriteData[4];
    int Mask = 1;
    int i;

    TickType_t Initial_Time = 0;
    TickType_t Time_Result;
    Task_Time[2] = 0;
  /* Infinite loop */
  for(;;)
  {
	  Initial_Time = osKernelSysTick();

	  osMutexWait(OutputMutexHandle, 1000);

	  /* Convert the value of Digital_Output variable to the Hardware outputs */
      for(i =0; i < 4; i++)
      {
          WriteData[i] = (Digital_Output & Mask);

          if(WriteData[i] > 0)
          {
              WriteData[i] = GPIO_PIN_SET;
          }
          else
          {
              WriteData[i] = GPIO_PIN_RESET;
          }


          Mask = Mask << 1;
      }
      Mask = 1;
	  
	  HAL_GPIO_WritePin(GPIOA, Digital_Output_3_Pin, WriteData[3]);
	  HAL_GPIO_WritePin(GPIOB, Digital_Output_2_Pin, WriteData[2]);
	  HAL_GPIO_WritePin(GPIOB, Digital_Output_1_Pin, WriteData[1]);
	  HAL_GPIO_WritePin(GPIOB, Digital_Output_0_Pin, WriteData[0]);

	  osMutexRelease(OutputMutexHandle);

	osDelay(1);
	/* Save task Time_Result at Task_Time vector*/
	osMutexWait(ScanMutexHandle, 1000);
	Time_Result =  osKernelSysTick() - Initial_Time;
	Task_Time[2] = Task_Time[2] + Time_Result;
	// printf("Task_Time[2]: %d\r\n", Task_Time[2]);
	osMutexRelease(ScanMutexHandle);

	 osDelay(10);
  }
  /* USER CODE END StartOutputUpdate */
}

/* StartDisplayUpdate function */
void StartDisplayUpdate(void const * argument)
{
  /* USER CODE BEGIN StartDisplayUpdate */
  uint8_t StrBuffer[20];
  uint8_t StrBin[20];

  TickType_t Final_Time = 0;
  TickType_t Initial_Time = 0;
  TickType_t Time_Result;
  Task_Time[3] = 0;
  // /* Infinite loop */
   for(;;)
   {
	  Initial_Time = osKernelSysTick();

	  /* SPI MUTEX - Display */
	 osMutexWait(SPIMutexHandle, 1000);

	if(Error_Flag == 0)
	{
		/* Convert Digital_Input to string (binary format) */
		DecimalToBin4bits(Digital_Input, StrBin);
		sprintf (StrBuffer, "DI: %sb", StrBin);
		LCD_Write_String(0,0,StrBuffer);

		/* Convert Digital_Output to string (binary format) */
		DecimalToBin4bits(Digital_Output, StrBin);
		sprintf (StrBuffer, "DO: %sb", StrBin);
		LCD_Write_String(0,1,StrBuffer);

		/* Convert the analog reading to string (decimal format) */
		sprintf (StrBuffer, "Analog: %d", Analog_Input);
		LCD_Write_String(0,2,StrBuffer);

		osMutexWait(ScanMutexHandle, 1000);

		/* Convert the ScanTimeGlobal variable to string (decimal format) */
		sprintf (StrBuffer, "Scan: %dd", ScanTimeGlobal);
		LCD_Write_String(0,3,StrBuffer);
		osMutexRelease(ScanMutexHandle);
	 }
	else
	{
		sprintf (StrBuffer, "             ");
		LCD_Write_String(0,0,StrBuffer);
		sprintf (StrBuffer, "#############");
		LCD_Write_String(0,1,StrBuffer);
		LCD_Write_String(0,2,StrBuffer);
		LCD_Write_String(0,4,StrBuffer);
		sprintf (StrBuffer, "             ");
		LCD_Write_String(0,5,StrBuffer);
		sprintf (StrBuffer, "#### ERRO ###");
		LCD_Write_String(0,3,StrBuffer);
	}

	 osMutexRelease(SPIMutexHandle);


	osDelay(1);
	/* Save task Time_Result at Task_Time vector*/
	osMutexWait(ScanMutexHandle, 1000);
	Time_Result =  osKernelSysTick() - Initial_Time;
	Task_Time[3] = Task_Time[3] + Time_Result;
	// printf("Task_Time[3]: %d\r\n", Task_Time[3]);
	osMutexRelease(ScanMutexHandle);

	osDelay(10);
  }
  /* USER CODE END StartDisplayUpdate */
}

/* StartHouseKeeping function */
void StartHouseKeeping(void const * argument)
{
  /* USER CODE BEGIN StartHouseKeeping */
	TickType_t Final_Time = 0;
	TickType_t Initial_Time = 0;
	TickType_t Time_Result;
	TickType_t ScanTime = 0;
	int AVG_Sample = 30;
	TickType_t AVG[AVG_Sample];
	int AVG_Count = 0;
	TickType_t AVG_Sum = 0;
	Task_Time[4] = 0;
	int debug_count = 0;
	TickType_t Task_Time_Previous[NUMBER_OF_TASKS];

	int i;

	/* Initial state of Task_Time_Anterior vector*/
	for(i = 0; i < NUMBER_OF_TASKS; i++)
	{
		Task_Time_Previous[i] = 0;
	}

	/* Initial state of AVG vector*/
	for(i = 0; i < AVG_Sample; i++)
	{
		AVG[i] = 0;
	}


  /* Infinite loop */
  for(;;)
  {
 Initial_Time = osKernelSysTick();

	/* Save task Time_Result at Task_Time vector*/
	osMutexWait(ScanMutexHandle, 1000);

	/* Sum all tasks time until this moment*/
	/* Note: ScanTime will be reseted only when all tasks had been executed at least one time
	 * So, if a tasks appears two times the ScanTime will count its time twice.
	 * */
	for(i = 0; i < NUMBER_OF_TASKS; i++)
	{
		/* This "if" below ensure that only new values are stored in ScanTime variable */
		if(Task_Time[i] > Task_Time_Previous[i])
		{
			ScanTime = ScanTime + Task_Time[i];
		}
		Task_Time_Previous[i] = Task_Time[i];
	}


	/* Reset Task_Time vector and ScanTime after all of them had been collected (executed at least one time) */
	if((Task_Time[0] > 0) && (Task_Time[1] > 0) && (Task_Time[2] > 0) && (Task_Time[3] > 0) && (Task_Time[4] > 0))
	{

		/* Reset average sum and sample counter*/
		if(AVG_Count >= (AVG_Sample - 1))
		{
			ScanTimeGlobal = (AVG_Sum / AVG_Sample);
			AVG_Sum = 0;
			AVG_Count = 0;
		}

		/* Fill up AVG buffer until it reaches the number of samples*/
		AVG[AVG_Count] = ScanTime;
		AVG_Sum = AVG_Sum + AVG[AVG_Count];
		AVG_Count++;

		/* Compare ScanTime to user ScanTimeLimit*/
		if(ScanTimeGlobal > ScanTimeLimit())
		{
			/* Raise ERROR */
			// printf("ScanTime: %d\r\n", ScanTime);
			Error_Flag = 1;
		}

		// ScanTimeGlobal = ScanTime;
		// debug_count++;
		// printf("debug_count: %d\r\n", debug_count);

		/* Reset the values of ScanTime and TaskTime variables */
		ScanTime = 0;
		for(i = 0; i < NUMBER_OF_TASKS; i++)
		{
			Task_Time[i] = 0;
		}
	}

	osMutexRelease(ScanMutexHandle);


	osDelay(1);
	/* Save task Time_Result at Task_Time vector*/
	osMutexWait(ScanMutexHandle, 1000);
	Time_Result =  osKernelSysTick() - Initial_Time;
	Task_Time[4] = Task_Time[4] + Time_Result;
	// printf("Task_Time[4]: %d\r\n", Task_Time[4]);
	osMutexRelease(ScanMutexHandle);

	osDelay(10);
  }
  /* USER CODE END StartHouseKeeping */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
