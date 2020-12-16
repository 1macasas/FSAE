/*
 * APP_Motores.h
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */

#ifndef APP_MOTORES_H_
#define APP_MOTORES_H_

// ESTADOS APP
#include "board.h"

/*global functions*/
void control_motors(int throttle,int direction,int brake);
double promedio(int valor_ingresado);
//int Pc_Communication(uint16_t Data[]);

/*extern variables*/



#endif /* APP_MOTORES_H_ */
