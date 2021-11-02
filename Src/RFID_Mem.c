#include "main.h"
#include "RFID_Mem.h"



bool Mem_Write(uint16_t Address ,void *data ,size_t dataSize)
{
	if(HAL_I2C_Mem_Write(&DevAddress ,MemAddress ,Address ,MemAddSize ,(uint8_t*)data ,dataSize ,100) == HAL_OK)
	{
		HAL_Delay(7);
		return true;
	}
	else
		return false;
}

bool Mem_Read(uint16_t Address ,void *data ,size_t dataSize)
{
	if(HAL_I2C_Mem_Read(&DevAddress ,MemAddress ,Address ,MemAddSize ,(uint8_t*)data ,dataSize ,100) == HAL_OK)
		return true;
	else
		return false;
}
