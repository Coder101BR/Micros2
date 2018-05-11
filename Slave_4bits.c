#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Read_Address_Request(int *AddrRead);
void Read_Data_of_Address(int AddrRead, int VectorData[]);

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

#define READ_FLAG 1
#define WRITE_FLAG 1

int main()
{

    int CS = 0;
    int Flag = READ_FLAG;
    int Address;
    int VectorData[16] = {0,0,0,0,
                          0,0,0,0,
                          0,0,0,0,
                          0,0,0,0};


    VectorData[0] = 5;
    VectorData[6] = 7;
    VectorData[7] = 15;
    VectorData[8] = 3;
    VectorData[15] = 14;


    if(CS == 0)
    {
        Read_Address_Request(&Address);

        if(Flag == READ_FLAG)
        {
            Read_Data_of_Address(Address, VectorData);
        }

        else if(Flag == WRITE_FLAG)
        {

        }


    }





    return 0;
}

void Read_Address_Request(int *AddrRead)
{
    /* Clean variable */
    *AddrRead = 0;

    /* Read data from HW and put it in a array */
    int ArraySimulation[4] ={1,1,1,1};


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
