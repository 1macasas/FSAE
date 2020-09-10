/*
 * mini_backend.c
 *
 *  Created on: Sep 3, 2020
 *      Author: manum
 */


#include "mini_backend.h"
/* primero vamos escribir el vector UART a enviar, aca es donde podemos elegir la informacion a enviar.
 * Primero elegimos las variables EXTERN a tomar y concatenar, el largo del vector esta determinado
 * por la variable UART_SIZE.
 *  */
extern uint16_t status[2];
extern uint16_t info_motores[18];
extern int throttle,direction,brake;

//velocidad de motor 1,motor 2, crr 1, crr 2, temp 1 , temp 2 ,torq1 , torq2
void preparo_data_uart(uint16_t vector[])
{
	vector[0]=info_motores[0];		//Velocidad motor 1
	vector[1]=info_motores[1];		//Velocidad motor 2
	vector[2]=info_motores[8];		//corriente motor 1
	vector[3]=info_motores[9];		//corriente motor 2
	vector[4]=info_motores[6];		//temperatura motor 1
	vector[5]=info_motores[7];		//temperatura motor 2
	vector[6]=info_motores[10];		//torque motor 1
	vector[7]=info_motores[11];		//torque motor 2
	vector[8]=throttle;				//Posicion acelerador
	vector[9]=brake;				//Freno
	vector[10]=direction;			//Volante
	vector[11]=info_motores[14];	//VELOCIDAD ACTUAL DEL AUTO, VALOR INVENTADO, MODIFICAR.
}
