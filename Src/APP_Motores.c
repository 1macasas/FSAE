/*
 * APP_Motores.c
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */

#include "APP_Motores.h"
#include "DRV_Motor_config.h"
#include "mini_backend.h"
#include "main.h"

/*private variables*/
											//ms
/// variables externas
extern uint8_t	buffer_Rx_msg[8];
extern CAN_RxHeaderTypeDef pRxHeader;
extern uint16_t status[2];
extern int velocity[2],DcCurrent[2],MotorCrr[2],VelocityAVG[2];
extern uint8_t MotorTemp[2],ControllerTemp[2];
extern short int Phase_A_Crr[2],Phase_B_Crr[2],DcLink[2],Torque[2],Warnings[2];
extern uint8_t FLAG_OP;
extern uint8_t FLAG_USART,FLAG_CAN;
extern TIM_HandleTypeDef htim2;
extern uint32_t ADC_val[ENTRADAS_ADC];

/******************************/
/*La funcion va a manejar la info de los motores
 * y el status (variables del main), y se va a encargar de controlar
 * los motores y actualizar la misma */
 extern UART_HandleTypeDef huart3;

void control_motors(int throttle,int direction,int brake)
{
	/*control_nodos es una variable se encarga de enviar la controlword a cada nodo, el contador va a controlar la data a pedir de los nodos*/
	static uint8_t nodo=MOTOR1,control_nodos[2];
	uint32_t Id;
	uint8_t buffer_analisis[8],i=0,bytes;

	//	ANALIZO MENSAJE CAN-- SI HAY
	//Armo un buffer de lectura para que no se mezclen los mensajes si llega uno neuvo, luego bajo el flag y analizo el mensaje
	if(FLAG_CAN==PENDING)
	{
	FLAG_CAN=READING;
	Id=pRxHeader.StdId;
	bytes=pRxHeader.DLC;
	for(i=1;i<8;i++)
	{
		buffer_analisis[i]=buffer_Rx_msg[i];
	}

	FLAG_CAN=READY;								//bajo el flag
	analize_CAN_Rx(Id,bytes,buffer_analisis);
	}

	if(FLAG_USART==49)					// SI TENGO PEDIDO DE ENVIO UART LO REALIZO
	{
		preparo_y_envio_data_uart(); 	//primero acomodo y elijo la info a enviar
		FLAG_USART=0;
	}

	// tiempos
	/* debo seleccionar alternadamente el motor a controlar, arranco la variable en 155 para
	 * saber que es la primera vez que la inicio y despues la alterno entre los nodos*/
	switch (status[nodo-1])
	{
		case(INITIALIZATION):		// configuro ambos nodos y arranco mi base de tiempo.
			control_nodos[0]=NOTSEND;
			control_nodos[1]=NOTSEND;
			status[0]=WFBOOTUP;
			status[1]=WFBOOTUP;
			break;

		case(WFBOOTUP):
				//ESTOY ESPERANDO QUE LOS NODOS ESTEN READY
				break;

		case(GOTOPREOPERATIONAL):
				if (status[0]==GOTOPREOPERATIONAL && status[1]==GOTOPREOPERATIONAL)		// Si estan ambos iniciados
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);		// prendo led verde
					send_NMT(PREOPERATIONAL);
					status[0]=PREOPERATIONAL;
					status[1]=PREOPERATIONAL;
				}
				break;
		case(PREOPERATIONAL):

					if(FLAG_OP==1)
					{
						if(send_NMT(START)!=HAL_OK)
						{
							Error_Handler();
							return;
						}

						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);				// enciendo luz azul en caso operacional
						status[0]=OPERATIONAL;
						status[1]=OPERATIONAL;
						FLAG_OP=0;											// UNA VEZ ENVIADO EL MSJ NMT BAJO EL FLAG
					}


			break;
	/*Caso OPERATIONAL y Ctrl WORD enviada:*/
		case(OPERATIONAL):
					if(control_nodos[nodo-1]==NOTSEND)
					{
						control_nodos[nodo-1]=change_state(nodo,OPERATIONAL,status[nodo-1] );
					}

				if(run_motor_n(ADC_val[0],ADC_val[1])!=HAL_OK)
					{
						Error_Handler();
						return;
					}

					if(FLAG_OP==1)
					{
						status[0]=STOPPED;
						status[1]=STOPPED;
						FLAG_OP=0;
					}
					break;

		case(STOPPED):
				control_nodos[0]=NOTSEND;
				control_nodos[1]=NOTSEND;
				status[0]=PREOPERATIONAL;
				status[1]=PREOPERATIONAL;
				if(send_NMT(RESTART)!=HAL_OK)
				{
					Error_Handler();
					return;
				}
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
				break;
	}
	/* Alterno los nodos entre el motor 1 y 2 a manejar
	 */
	if(nodo==MOTOR1)
	{
		nodo=MOTOR2;
	}
	else if (nodo==MOTOR2)
	{
		nodo=MOTOR1;
	}

	//PIDO INFO (CONSTANTEMENTE)
	ask_for_info();

 }


