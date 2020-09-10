/*
 * board.h
 *
 *  Created on: 3 mar. 2020
 *      Author: Manuel
 */

#ifndef BOARD_H_
#define BOARD_H_

//private #defines
// PINES USADOS
//estados
#define INITIALIZATION	0
#define WFBOOTUP		1
#define PREOPERATIONAL 	2
#define OPERATIONAL 	3
#define STOPPED			4

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
#define ENTRADAS_ADC 3


#endif /* BOARD_H_ */
