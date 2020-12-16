/*
 * board.h
 *
 *  Created on: 3 mar. 2020
 *      Author: Manuel
 */

#ifndef BOARD_H_
#define BOARD_H_

//INCLUYO DEFINES COMUNES UTILES
#include "stdint.h"

//private #defines
// PINES USADOS
//estados
#define INITIALIZATION		0
#define WFBOOTUP			1
#define GOTOPREOPERATIONAL 	2
#define PREOPERATIONAL 		3
#define OPERATIONAL 		4
#define STOPPED				5

//nodos
#define MOTOR1 1
#define MOTOR2 2

//comunicacion protocolo can sdo
#define n_MSK (0x0C)
#define s_MSK (0x01)
#define e_MSK (0x02)
#define ccs_MSK (0xE0)

//controlword
#define NOTSEND 0
#define SEND 1
#define SEMI_SEND 2

//UART
#define UART_SIZE 12
#define Info_motores_size 18
//
#define ENTRADAS_ADC 2

//OBJECT INDEX
#define VELOCITY 		0x6C60
#define DCLINK 			0x7960
#define DC_CURRENT 		0x2320
#define MOTOR_TEMP 		0x2520
#define MOTOR_CRR 		0x7860
#define TORQUE 			0x7760
#define VELOCITY_AVG 	0x8620
#define CONTROLLER_TEMP 0x2620
#define VELOCITY_ACTUAL 0x2087

// CAN STATUS
#define READY			0
#define PENDING 		1
#define READING 		2

//nmt
#define START 			1
#define RESTART 		0

#endif /* BOARD_H_ */
