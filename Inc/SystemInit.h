/*
 * SystemInit.h
 *
 *  Created on: Jun 22, 2020
 *      Author: manum
 */

#ifndef SYSTEMINIT_H_
#define SYSTEMINIT_H_

//Includes
#include "main.h"
#include "board.h"

// extern variables

extern uint16_t status[2];

//ADC
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern uint32_t ADC_val[ENTRADAS_ADC];
//CAN
extern CAN_HandleTypeDef hcan1;
// TIMER
extern TIM_HandleTypeDef htim2;

//FLAGS
extern uint8_t FLAG_OP;
extern uint8_t FLAG_CAN;
extern uint8_t FLAG_USART;

//UART
extern uint8_t	 UART_Tx_Data[];
extern uint8_t  UART_RX_DATA[UART_SIZE];
extern uint16_t  Size_Tx_Data;
extern UART_HandleTypeDef huart3;
//DMA_HandleTypeDef hdma_usart3_tx;
//DMA_HandleTypeDef hdma_usart3_rx;

//
void mysystem_start(void);

#endif /* SYSTEMINIT_H_ */
