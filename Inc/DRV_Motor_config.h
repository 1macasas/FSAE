/*
 * Motor.h
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */
/*Funciones propias que realizan acciones sobre el motor*/

#ifndef MOTOR_H_
#define MOTOR_H_

#include "board.h"
#include "main.h"
#include "DRV_comunication.h"

/*private functions*/
int send_ctrlwrd_1(uint16_t node);
int send_ctrlword_2(uint16_t node);
int run_motor_n(uint16_t node,uint32_t in,uint16_t state);
int ask_for_info(uint8_t nodo , uint8_t contador);
int analize_CAN_Rx(CAN_RxHeaderTypeDef header ,uint8_t mensaje []);

/*extern functions*/
extern int can1_Tx(uint16_t Id ,uint8_t mensaje [],uint8_t bytes);

#endif /* MOTOR_H_ */
