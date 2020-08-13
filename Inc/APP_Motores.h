/*
 * APP_Motores.h
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */

#ifndef APP_MOTORES_H_
#define APP_MOTORES_H_

#include "main.h"
#include "DRV_Motor_config.h"
#include "board.h"
// ESTADOS APP

/*global functions*/
void control_motors(int throttle,int direction,int brake);
double promedio(int valor_ingresado);
int Pc_Communication(uint16_t Data[]);

/*extern variables*/

extern uint8_t	buffer_Rx_msg[8];
extern CAN_RxHeaderTypeDef pRxHeader;
extern uint16_t status[2];
extern uint16_t  info_motores[18];
extern uint8_t FLAG_OP;
extern uint8_t FLAG_USART;
extern TIM_HandleTypeDef htim2;


#endif /* APP_MOTORES_H_ */
