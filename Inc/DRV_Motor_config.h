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

/*private functions*/
int send_ctrlwrd_1(uint16_t node);
int send_ctrlword_2(uint16_t node);
char run_motor_n(uint32_t pedal1,uint32_t pedal2);
int ask_for_info(void);
//int analize_CAN_Rx(CAN_RxHeaderTypeDef header ,uint8_t mensaje []);
char analize_CAN_Rx(uint32_t Id,uint8_t bytes,uint8_t mensaje []);
char send_NMT(uint8_t command);
char change_state(uint8_t nodo,uint8_t state, uint8_t status);
/*extern functions*/
//extern int can1_Tx(uint16_t Id ,uint8_t mensaje [],uint8_t bytes);

#endif /* MOTOR_H_ */
