/*
 * UART_COM.c
 *
 *  Created on: Dec 9, 2020
 *      Author: manum
 */

#include "UART_COM.h"
extern UART_HandleTypeDef huart3;

int Pc_Communication(uint16_t Data[])
{
	//uint16_t  size = sizeof(Data) / sizeof(Data[0]);
	// esto puede explotar, ver que concuerden los tamaños a analizar.
		//hago un casteo a info_motores. EL DMA transmite 8-16 bits
	if(HAL_UART_Transmit_DMA(&huart3, Data, UART_SIZE) != HAL_OK)
	{
		return HAL_ERROR;
	}
		return HAL_OK;

}
