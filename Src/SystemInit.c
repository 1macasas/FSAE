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
	  HAL_ADC_Start_DMA(&hadc1,ADC_val, 2);

	  HAL_TIM_Base_Start_IT(&htim2);




		/*Inicio*/
	  status[0]= INITIALIZATION;
	  status[1]= INITIALIZATION;
	  FLAG_OP=0;

}


