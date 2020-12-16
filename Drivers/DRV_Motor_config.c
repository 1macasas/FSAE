/*
 * Motor.c
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */


#include "DRV_Motor_config.h"
#include "DRV_CAN.h"

extern uint16_t status[2];
extern int velocity[2],DcLink[2],DcCurrent[2],MotorTemp[2],MotorCrr[2],Torque[2],VelocityAVG[2],ControllerTemp[2],VelocityAct[2];

/*La funcion pone en movimiento el motor del nodo indicado, debe recibir
 *  el valor del nodo (1 o 2), el valor al cual se desea mover y el estado del nodo,
 *  si el nodo no se encuentra en estado operacional no se realiza accion*/

char run_motor_n(uint32_t pedal1,uint32_t pedal2)
{
	static int now=0,last=0,node=1;
	uint32_t v,a,b,c,d;
	uint8_t VEL[8]; //velocidad a enviar por can.
		now=HAL_GetTick();
		// Actualizo valor de Velocidad
		if (now-last>50)			//ms??
		{
		if (node==1)
			v=(pedal1*500)/4095;
		else if(node==2)
			v=(pedal2*500)/4095;

		if (v<10)
			v=0;

		a=((v)&(0x000000FF));
		b=((v)&(0x0000FF00))>>(8);
		c=((v)&(0x00FF0000))>>(16);
		d=((v)&(0xFF000000))>>(24);
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
		last=now;
		node++;
		if(node==3)
			node=1;
		}
		else if (now<last)
			last=0;
		else
		{
			return HAL_OK;
		}
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
char analize_CAN_Rx(uint32_t Id,uint8_t DLC,uint8_t mensaje [])
{
	uint32_t info,volatil;
	uint16_t index,command_byte=0;		//,subindex
	uint8_t ccs,contador,nodo;

	/*Vamos a analizar casos: mensaje de bootup, y mensaje sdo, podriamos agregar pdo en un futuro*/
	/*ORDEN DE REVISION: MENSAJES DE MAYOR A MENOR ID*/
		if(Id>0x700)
		{
			nodo=Id-0x700;
			if(status[nodo-1]==WFBOOTUP)
				status[nodo-1]=GOTOPREOPERATIONAL;
		}
		// con revisar que sea mayor a cero  me alcanza porque sino hubiese entrado en el caso anterior
		else if (Id>0x600)
			// ESTO NO DEBERIA PASAR, FILTRAR ANTES
		{
			return HAL_ERROR;
		}
		else if (Id>0x580)
			// primero debo analizar el byte de comando
			// Analisis CANOPEN
		{
		nodo=Id-0x580;
		index= (mensaje[1]<<(8))|mensaje[2]	;
		command_byte=mensaje[0];
		ccs=(command_byte&ccs_MSK)>>(5);
		info=0;
		volatil=0;
		//e=(mensaje[0]&e_MSK);
		//s=(mensaje[0]&s_MSK);
		//if(ccs!=2)
		//	return HAL_OK;			// NO HAY ANALISIS POSIBLE EN ESTE MENSAJE
		//
		if(nodo<0)
			return HAL_ERROR;

		if(ccs==3)					// ESTA VERIFICACION NO DEBERIA ESTAR
		{
			return HAL_OK;
		}
			for(contador=1;contador<(5);contador++)
			{
				volatil= mensaje[DLC-contador];
				info=((info<<8)|volatil);

			}
			if(info==101253137)
			{
					return HAL_ERROR;
			}
			if (index==VELOCITY)	        //velocity actual value
			{
				velocity[nodo-1]=info;
			}
			else if (index==DCLINK)	    //DC Link actual voltage
			{
				DcLink[nodo-1]=info;
			}
			else if (index==DC_CURRENT) 	//DC Current
			{
				DcCurrent[nodo-1]=info;
			}
			else if (index==MOTOR_TEMP)		//Motor Temp
			{
				MotorTemp[nodo-1]=info;
			}
			else if (index==MOTOR_CRR)		// Motor Current Actual Value
			{
				MotorCrr[nodo-1]=info;
			}
			else if (index==TORQUE)		//Torque Actual Value
			{
				Torque[nodo-1]=info;
			}
			else if (index==0x6C60)		//Velocity actual Value
			{
				VelocityAct[nodo-1]=info;
			}
			else if (index==VELOCITY_AVG)		// Velocity actual value AVG
			{
				VelocityAVG[nodo-1]=info;
			}
			else if (index==CONTROLLER_TEMP)		//controller temperature
			{
				ControllerTemp[nodo-1]=info;
			}
		}

	  return HAL_OK ;
}

/*La funcion envia el mensaje SDO para pedir data al nodo indicado*/

int ask_for_info(void)
{
	/*inicio la variable en cero, voy a pedir 9 datos*/
	static int now=0,last=0;
	static uint8_t node=1,contador=1;
	uint8_t PIDO_INFO[8];
	/*primero command byte, luego index y subindex*/
	now=HAL_GetTick();
	if (now-last>10)			//ms??
	{
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


	if(can1_Tx(0x600+node,PIDO_INFO,8)==HAL_OK)
			{
				last=now;
				node++;
				if(node==3)
					node=1;
				contador++;
					if(contador>10)
						contador=0;
				return HAL_OK;
			}
	}

}

char send_NMT(uint8_t command)
{
	/*Envio mensaje NMT para pasar a OPERACIONAL-PREOPERACIONAL*/
	uint8_t NMT[2];
	NMT[1]=0;
	if(command==START)
		NMT[0]=0x01;		        //START
	else if(command==RESTART)
		NMT[0]=0X81;
	else if(command==PREOPERATIONAL)
		NMT[0]=0X80;
	can1_Tx(0x000,NMT,2);

	HAL_Delay(20);			//DELAY BLOQUEANTE

	return HAL_OK;
}

char change_state(uint8_t nodo,uint8_t state, uint8_t status)
{
		HAL_Delay(100);
		if(send_ctrlwrd_1(nodo)== HAL_OK)
			//control_nodos[nodo-1]=SEMI_SEND;

		HAL_Delay(100);			//DELAY BLOQUEANTE

		if(send_ctrlword_2(nodo)==HAL_OK)
			//control_nodos[nodo-1]=SEND;

		HAL_Delay(100);			//DELAY BLOQUEANTE
		return SEND;
}
