#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Convert_UserCMD_To_Int(char *Str, int *RW, int *Data, int *Address);
void Write_Data(int Data, int bits, int Write_Permission);
void Write_Address(int Data, int bits);
void ChipSelect(int CS);
void Read_Data(int *DataRead, int Read_Permission);

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

int main()
{

    int ReadWrite = 55;
    int Data;
    int Address;
    int ReadData = 66;

    /* Simulation of a RS232 string */
    char TestStr[15] = {"r,12,10"};
    //char TestStr[15] = {"r,12"};

    Convert_UserCMD_To_Int(TestStr, &ReadWrite, &Data, &Address);

    printf("\nReadWrite: %d", ReadWrite);
    printf("\nData: %d", Data);
    printf("\nAddress: %d", Address);


    Write_Address(Address, 4);

    if(ReadWrite == 0)
    {
        Write_Data(Data, 4, ReadWrite);
        ChipSelect(0); /* Enable CS signal */
        /* wait some time */
        ChipSelect(1); /* Disable write line, read line, cs line and put every data to input */
    }

    else if(ReadWrite == 1)
    {
        ChipSelect(0);
        Read_Data(&ReadData, ReadWrite);
        /* Ler na serial */
        printf("\nReadData: %d", ReadData);
        ChipSelect(1); /* Disable write line, read line, cs line and put every data to input */
    }



    return 0;
}

void Convert_UserCMD_To_Int(char *Str, int *RW, int *Data, int *Address)
{
    char StrAux[3];
    /* Update *RW */
    if((Str[0] == 'w') | (Str[0] == 'W'))
    {
        /* Set *RW to write state*/
        *RW = 0;

        /* Update Data */
        StrAux[0] = Str[2];
        StrAux[1] = Str[3];
        *Data = atoi(StrAux); /* Convert string to int */

        /* Update Address */
        StrAux[0] = Str[5];
        StrAux[1] = Str[6];
        *Address = atoi(StrAux); /* Convert string to int */

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
        /* Ou não faz nada e seta *RW igual a um valor diferente*/
      *RW = 1;
      printf("\nInvalid command");
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

            printf("\nWriteData[%d] = %d", i,WriteData[i]); // Substituir para escrever nos GPIO
            Mask = Mask << 1;
        }

        // Set 0 to write line and Set 1 to read line
    }

}

void Read_Data(int *DataRead, int Read_Permission)
{

    if(Read_Permission == 1)
    {
        // Hardware: Set 1 to write line and Set 0 to read line

        /* Clean variable */
        *DataRead = 0;

        /* Read data from HW and put it in a array */
        int ArraySimulation[4] ={1,1,1,0};

        *DataRead = ((ArraySimulation[0] << 3) | (ArraySimulation[1] << 2) | (ArraySimulation[2] << 1) | (ArraySimulation[3] << 0));


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

        printf("\nAddress[%d] = %d", i,WriteData[i]); // Substituir para escrever nos GPIO
        Mask = Mask << 1;
    }
}

void ChipSelect(int CS)
{
    /* Select the device to write/read*/
    if(CS == 0)
    {
        printf("\nCS = ZERO");
    }
    else
    {
        printf("\nCS = UM");
    }


}
