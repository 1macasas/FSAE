/*
 * APP_Motores.c
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */

#include "APP_Motores.h"

/*private variables*/
//cycles va a tener la relacion entre pido data-envio velocidad.
 int cycles=0;
 char freq=2; //ms

/******************************/
/*La funcion va a manejar la info de los motores
 * y el status (variables del main), y se va a encargar de controlar
 * los motores y actualizar la misma */
 extern UART_HandleTypeDef huart3;

void control_motors(int throttle,int direction,int brake)
{
	/*control_nodos es una variable se encarga de enviar la controlword a cada nodo, el contador va a controlar la data a pedir de los nodos*/
	static uint8_t nodo=MOTOR1,control_nodos[2],contador[2];
	static int tickstart;

	uint8_t NMT[2];
	NMT[1]=0;
	//
	if(FLAG_CAN==1)
	{
	analize_CAN_Rx(pRxHeader,buffer_Rx_msg);
	FLAG_CAN=0;			//bajo el flag
	}

	if(FLAG_USART==49)
	{
		Pc_Communication(info_motores);
		FLAG_USART=0;
	//	HAL_UART_Receive_DMA(&huart3,&FLAG_USART,1);
	}

	// tiempos
	/* debo seleccionar alternadamente el motor a controlar, arranco la variable en 155 para
	 * saber que es la primera vez que la inicio y despues la alterno entre los nodos*/
	switch (status[nodo-1])
	{
		case(INITIALIZATION):		// configuro ambos nodos y arranco mi base de tiempo.
			control_nodos[0]=NOTSEND;
			control_nodos[1]=NOTSEND;
			contador[0]=1;
			contador[1]=1;
			status[0]=WFBOOTUP;
			status[1]=WFBOOTUP;
			tickstart= HAL_GetTick();
			break;

		case(WFBOOTUP):
				//UN VEZ QUE LLEGA MSJ BOOTUP
				break;

		case(PREOPERATIONAL):

				if (status[0]==PREOPERATIONAL && status[1]==PREOPERATIONAL)		// Si estan ambos en operational
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);		// prendo led verde

					if(FLAG_OP==1)
					{
						NMT[0]=0x01;		        //Pasar a operacional

						if(can1_Tx(0x000,NMT,2)!=HAL_OK)
						{
							Error_Handler();
							return;
						}
						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);				// enciendo luz azul en caso operacional
						status[0]=OPERATIONAL;
						status[1]=OPERATIONAL;
						FLAG_OP=0;											// UNA VEZ ENVIADO EL MSJ NMT BAJO EL FLAG
					}

			}

			break;
	/*Caso OPERATIONAL y Ctrl WORD enviada:*/
		case(OPERATIONAL):
					if(control_nodos[nodo-1]==NOTSEND)		// Pulso de pasar de preoperational a operational
					{
						if( (HAL_GetTick()-tickstart>freq) )
						{
						if(send_ctrlwrd_1(nodo)== HAL_OK)
							control_nodos[nodo-1]=SEMI_SEND;
					}
					else if(control_nodos[nodo-1]==SEMI_SEND)
					{
						if(send_ctrlword_2(nodo)==HAL_OK)
							control_nodos[nodo-1]=SEND;
					}
					else if(FLAG_OP==1)
					{
						status[0]=STOPPED;
						status[1]=STOPPED;
						FLAG_OP=0;
					}
						tickstart=HAL_GetTick();
					}
					else if (cycles==5)
					{
						run_motor_n(nodo,throttle,status[nodo-1]);
						cycles++;
					}
					break;

		case(STOPPED):
				control_nodos[0]=NOTSEND;
				control_nodos[1]=NOTSEND;
				status[0]=PREOPERATIONAL;
				status[1]=PREOPERATIONAL;
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
				break;
	}
	/*Reseteo Variable control en el caso de volver a preoperational*/

	/* controlo alternadamente motor 1 y motor 2 --- RESETEO VARUABKES
	//control_nodos[nodo-1]=control;  // ???? en q esta ba pensando*/
	if(nodo==MOTOR1)
	{
		nodo=MOTOR2;
	}
	else if (nodo==MOTOR2)
	{
		nodo=MOTOR1;
	}

	//PIDO INFO (CONSTANTEMENTE)---ciclos¿?7

			if( (HAL_GetTick()-tickstart>freq) )
			{
				if((status[nodo-1]==OPERATIONAL && cycles==5) || (status[nodo-1]==PREOPERATIONAL && FLAG_OP==1))
				{

				}
				else
				{
					if(ask_for_info( nodo ,contador[nodo-1])==HAL_OK)
					{
						contador[nodo-1]++;
						cycles++;
						tickstart=HAL_GetTick();
						if(contador[nodo-1]>10)
						{
							contador[nodo-1]=1;
							cycles=0;
						}
					}
				}
			}
 }


