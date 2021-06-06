/*
 * SystemInit.c
 *
 *  Created on: Jun 22, 2020
 *      Author: manum
 */
//
#include "SystemInit.h"

extern int velocity[2],DcCurrent[2],MotorCrr[2],VelocityAVG[2];
extern uint8_t MotorTemp[2],ControllerTemp[2];
extern short int Phase_A_Crr[2],Phase_B_Crr[2],DcLink[2],Torque[2],Warnings[2];
CAN_FilterTypeDef sFilterConfig;

void mysystem_start(void)
{
	/*Inicio configuracion CAN*/

	  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	  HAL_CAN_Start(&hcan1);

		/*Inicio configuracion Filtro can*/
	 sFilterConfig.FilterFIFOAssignment=CAN_FILTER_FIFO0;
	 sFilterConfig.FilterIdHigh=0x245<<5;
	 sFilterConfig.FilterIdLow=0;
	 sFilterConfig.FilterMaskIdHigh=0;
	 sFilterConfig.FilterMaskIdLow=0;
	 sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT;
	 sFilterConfig.FilterActivation=CAN_FILTER_ENABLE;

	 HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);

	 /*Inicio ADC con DMA */
	  HAL_ADC_Start_DMA(&hadc1,ADC_val, ENTRADAS_ADC);

	  /*TIMER 2 PARA FUNCIONES.
	   * (Creo que no lo uso al final se puede eliminar
	   * */
	  HAL_TIM_Base_Start_IT(&htim2);

	  FLAG_USART=0;
	  /*Inicio USART3*/
	  HAL_UART_Receive_DMA(&huart3,&FLAG_USART,1);


		/*Inicio mis variables y FLAGS*/
	  status[0]= INITIALIZATION;
	  status[1]= INITIALIZATION;
	  FLAG_OP=0;
	  FLAG_CAN=0;


	  /// esto es para prueba con software, se tiene que borrar
	  velocity[0]=0xffff;
	  velocity[1]=0xffff;
	  DcLink[0]=0xffff;
	  DcLink[1]=0xffff;
	  DcCurrent[0]=0xffff;
	  DcCurrent[1]=0xffff;
	  MotorTemp[0]=0xff;
	  MotorTemp[1]=0xff;
	  MotorCrr[0]=0xffff;
	  MotorCrr[1]=0xffff;
	  Torque[0]=0xffff;
	  Torque[1]=0xffff;
	  VelocityAVG[0]=0xffff;
	  VelocityAVG[1]=0xffff;
	  ControllerTemp[0]=0xff;
	  ControllerTemp[1]=0xff;

}


