/*
 * mini_backend.c
 *
 *  Created on: Sep 3, 2020
 *      Author: manum
 */


#include "mini_backend.h"
#include "UART_COM.h"
/* primero vamos escribir el vector UART a enviar, aca es donde podemos elegir la informacion a enviar.
 * Primero elegimos las variables EXTERN a tomar y concatenar, el largo del vector esta determinado
 * por la variable UART_SIZE.
 *  */
extern uint16_t status[2];
extern int velocity[2],DcCurrent[2],MotorCrr[2],VelocityAVG[2];
extern uint8_t MotorTemp[2],ControllerTemp[2];
extern short int Phase_A_Crr[2],Phase_B_Crr[2],DcLink[2],Torque[2],Warnings[2];
extern int throttle,direction,brake;

//velocidad de motor 1,motor 2, crr 1, crr 2, temp 1 , temp 2 ,torq1 , torq2
void preparo_y_envio_data_uart(void)
{
	uint16_t info_UART[UART_SIZE];

	info_UART[0]=velocity[0];		//Velocidad motor 1
	info_UART[1]=velocity[1];		//Velocidad motor 2
	info_UART[2]=MotorCrr[0];		//corriente motor 1
	info_UART[3]=MotorCrr[1];		//corriente motor 2
	info_UART[4]=MotorTemp[0];		//temperatura motor 1
	info_UART[5]=MotorTemp[1];		//temperatura motor 2
	info_UART[6]=Torque[0];			//torque motor 1
	info_UART[7]=Torque[1];			//torque motor 2
	info_UART[8]=throttle;			//Posicion acelerador
	info_UART[9]=brake;				//Freno
	info_UART[10]=direction;		//Volante
	info_UART[11]=VelocityAVG[0];	//VELOCIDAD ACTUAL DEL AUTO, VALOR INVENTADO, MODIFICAR.

	Pc_Communication(info_UART);	//envio info
}
