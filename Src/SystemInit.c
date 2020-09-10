/*
 * SystemInit.c
 *
 *  Created on: Jun 22, 2020
 *      Author: manum
 */
//
#include "SystemInit.h"

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
	  sFilterConfig.FilterActivation=ENABLE;
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

	  uint8_t i;

	  /// esto es para prueba con software, se tiene que borrar
	  for(i=0;i<18;i++)
	  {
		  info_motores[i]=0xFFFF;
	  }

}


