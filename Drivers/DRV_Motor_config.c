/*
 * Motor.c
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */


#include "DRV_Motor_config.h"

extern uint16_t status[2];
extern uint16_t info_motores[18];

/*La funcion pone en movimiento el motor del nodo indicado, debe recibir
 *  el valor del nodo (1 o 2), el valor al cual se desea mover y el estado del nodo,
 *  si el nodo no se encuentra en estado operacional no se realiza accion*/
int run_motor_n(uint16_t node,uint32_t in,uint16_t state)
{
//	if(state==OPERATIONAL)
//	{
		// Actualizo valor de Velocidad
		uint8_t VEL[8]; //velocidad a enviar por can.
					int v=(in)*(5000/4095);
					uint32_t a=((v)&(0x000000FF));
					uint32_t b=((v)&(0x0000FF00))>>(8);
					uint32_t c=((v)&(0x00FF0000))>>(16);
					uint32_t d=((v)&(0xFF000000))>>(24);
		VEL[0]=0x23;
		VEL[1]=0xff;
		VEL[2]=0x60;
		VEL[3]=0x00;  	 		//subindex
		VEL[4]=a;
		VEL[5]=b;
		VEL[6]=c;
		VEL[7]=d;
		if(can1_Tx(0x600+node,VEL,8)!=HAL_OK)
				{
					Error_Handler();
					return HAL_ERROR;
				}
//		}
	return HAL_OK;
}

int send_ctrlwrd_1(uint16_t node)
{
	uint8_t CTRL_WORD[8]; // activar motores
	CTRL_WORD[0]=0x2B;
	CTRL_WORD[1]=0x40;
	CTRL_WORD[2]=0x60;
	CTRL_WORD[3]=0x00;
	CTRL_WORD[4]=0x06;
	CTRL_WORD[5]=0x00;
	CTRL_WORD[6]=0x00;
	CTRL_WORD[7]=0x00;
	if(can1_Tx(0x600+node,CTRL_WORD,8)!=HAL_OK)
	{
		Error_Handler();
		return HAL_ERROR;
	}
	return HAL_OK;
}

int send_ctrlword_2(uint16_t node)
{
	uint8_t CTRL_WORD[8]; // activar motores

	CTRL_WORD[0]=0x2B;
	CTRL_WORD[1]=0x40;
	CTRL_WORD[2]=0x60;
	CTRL_WORD[3]=0x00;
	CTRL_WORD[4]=0x0F;
	CTRL_WORD[5]=0x00;
	CTRL_WORD[6]=0x00;
	CTRL_WORD[7]=0x00;

	if(can1_Tx(0x600+node,CTRL_WORD,8)!=HAL_OK)
	{
		Error_Handler();
		return HAL_ERROR;
	}
	return HAL_OK;
}

// Recibo Mensaje-- debo pasarle las variables donde voy a guardar el ID, y el mensaje:
int analize_CAN_Rx(CAN_RxHeaderTypeDef header,uint8_t mensaje [])
{
	uint32_t info=0,volatil=0;
	uint16_t Id,bytes,index,subindex;
	uint8_t n,ccs,e,s,contador,nodo;

	Id=header.StdId;
	bytes=header.DLC;
	/*Vamos a analizar casos: mensaje de bootup, y mensaje sdo, podriamos agregar tpdo en un futuro*/
	/*ORDEN DE REVISION: MENSAJES DE MAYOR A MENOR ID*/
		if(Id-0x700>0)
		{
			nodo=Id-0x700;
			status[nodo-1]=PREOPERATIONAL;
		}
		// con revisar que sea mayor a cero  me alcanza porque sino hubiese entrado en el caso anterior
		else if (Id-580>0)
			// primero debo analizar el byte de comando
			// Analisis CANOPEN
		{
		nodo=Id-0x580;
		index= (mensaje[1]<<(8))|mensaje[2]	;
		subindex=mensaje[3];
		ccs=(mensaje[0]&ccs_MSK);
		n=(mensaje[0]&n_MSK);
		e=(mensaje[0]&e_MSK);
		s=(mensaje[0]&s_MSK);
			for(contador=(1+n);contador<(bytes-3);contador++)
			{
				volatil= mensaje[bytes-contador];
				info=(info<<8)+volatil;
			}

			if (index==0x2087)	        //velocity actual value
			{
				info_motores[nodo-1]=info;
			}
			else if (index==0x7960)	    //DC Link actual voltage
			{
				info_motores[1+nodo]=info;
			}
			else if (index==0x2320) 	//DC Current
			{
				info_motores[3+nodo]=info;
			}
			else if (index==0x2520)		//Motor Temp
			{
				info_motores[5+nodo]=info;
			}
			else if (index==0x7860)		// Motor Current Actual Value
			{
				info_motores[7+nodo]=info;
			}
			else if (index==0x7760)		//Torque Actual Value
			{
				info_motores[9+nodo]=info;
			}
			else if (index==0x6C60)		//Velocity actual Value
			{
				info_motores[11+nodo]=info;
			}
			else if (index==0x8620)		// Velocity actual value AVG
			{
				info_motores[13+nodo]=info;
			}
			else if (index==0x2620)		//controller temperature
			{
				info_motores[15+nodo]=info;
			}
		}

	  return HAL_OK ;
}

/*La funcion envia el mensaje SDO para pedir data al nodo indicado*/
int ask_for_info(uint8_t node , uint8_t contador)
{
	/*inicio la variable en cero, voy a pedir 9 datos*/
	uint8_t PIDO_INFO[8];
	/*primero command byte, luego index y subindex*/
	PIDO_INFO[0]=0x40;
	if (contador==1)	//velocity actual value
	{
				PIDO_INFO[1]=0x87;
				PIDO_INFO[2]=0x20;
	}
	else if (contador==2)	//DC Link Actual Voltage
	{
				PIDO_INFO[1]=0x79;
				PIDO_INFO[2]=0x60;
	}
	else if (contador==3)	//Dc Current
	{
				PIDO_INFO[1]=0x23;
				PIDO_INFO[2]=0x20;
	}
	else if (contador==4)	//Motor Temperature
	{
				PIDO_INFO[1]=0x25;
				PIDO_INFO[2]=0x20;
	}
	else if (contador==5)	//Motor current actual value
	{
				PIDO_INFO[1]=0x78;
				PIDO_INFO[2]=0x60;
	}
	else if (contador==6)	//Torque Actual Value
	{
				PIDO_INFO[1]=0x77;
				PIDO_INFO[2]=0x60;
	}
	else if (contador==7)	//velocity actual value
	{
				PIDO_INFO[1]=0x6C;
				PIDO_INFO[2]=0x60;
	}
	else if (contador==8)	//velocity actual value AVG
	{
				PIDO_INFO[1]=0x86;
				PIDO_INFO[2]=0x20;
	}
	else if (contador==9)	//controller temperature
	{
				PIDO_INFO[1]=0x26;
				PIDO_INFO[2]=0x20;
				PIDO_INFO[3]=0x01;
	}
	else if (contador==10)	//controller temperature
	{
				PIDO_INFO[1]=0x78;
				PIDO_INFO[2]=0x60;
				PIDO_INFO[3]=0xC1;
	}

	if(can1_Tx(0x600+node,PIDO_INFO,8)!=HAL_OK)
			{
				Error_Handler();
				return HAL_ERROR;
			}
	return HAL_OK;
}

