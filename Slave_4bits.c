#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Read_Address_Request(int *AddrRead);
void Read_Data_of_Address(int AddrRead, int VectorData[]);
void Write_Data_at_Address(int AddrRead, int VectorData[]);

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

#define READ_FLAG 1
#define WRITE_FLAG 0

int main()
{

    int CS = 0;
    int Flag = WRITE_FLAG;
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
            Write_Data_at_Address(Address, VectorData);

            int i = 0;

            for(i = 0; i < 16; i++)
            {
                printf("\nVectorData[%d]:  %d", i, VectorData[i]);
            }

        }


        /* Set all GPIOs to input*/
    }





    return 0;
}

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

    //int DataSentFromMaster[4] = {1,1,0,0};

    int DataSentFromMaster[4];
    /* Read data sent from master */

    DataSentFromMaster[0] = 1;
    DataSentFromMaster[1] = 1;
    DataSentFromMaster[2] = 0;
    DataSentFromMaster[3] = 1;


    VectorData[AddrRead] = ((DataSentFromMaster[0] << 3) | (DataSentFromMaster[1] << 2) | (DataSentFromMaster[2] << 1) | (DataSentFromMaster[3] << 0));

    printf("Inside the function: %d", VectorData[AddrRead]);

}
