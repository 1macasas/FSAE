/*
 * DRV_CAN.c
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */

/*includes*/
#include <DRV_CAN.h>

/*Private Variables*/
CAN_TxHeaderTypeDef pHeader;

/*Extern Variables*/
extern CAN_HandleTypeDef hcan1;
extern  uint32_t TxMailbox1,TxMailbox2,TxMailbox3;


// mi driver para enviar info por can
// le doy el ID y el mensaje a enviar, y me devuelve un 1 si se envio correctamente, o un cero si tuvo algun error


char can1_Tx(uint16_t Id ,uint8_t mensaje [],uint8_t bytes)
{

	  if(HAL_CAN_IsTxMessagePending(&hcan1, CAN_TX_MAILBOX0) == 0)
	  {
	  pHeader.DLC=bytes;
	  pHeader.IDE=CAN_ID_STD;
	  pHeader.RTR=CAN_RTR_DATA;
	  pHeader.StdId=Id;
	  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

		  if(HAL_CAN_AddTxMessage(&hcan1, &pHeader, mensaje, &TxMailbox1) != HAL_OK )
	  		{
	  				return HAL_ERROR;
	  		}
		  return HAL_OK;
	  }

	  else if(HAL_CAN_IsTxMessagePending(&hcan1, CAN_TX_MAILBOX1) == 0)
	  {
	  pHeader.DLC=bytes;
	  pHeader.IDE=CAN_ID_STD;
	  pHeader.RTR=CAN_RTR_DATA;
	  pHeader.StdId=Id;
	  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

		  if(HAL_CAN_AddTxMessage(&hcan1, &pHeader, mensaje, &TxMailbox2) != HAL_OK )
	  		{
	  				return HAL_ERROR;
	  		}
		  return HAL_OK;

	  }
	  else if(HAL_CAN_IsTxMessagePending(&hcan1, CAN_TX_MAILBOX2) == 0)
	  {
	  pHeader.DLC=bytes;
	  pHeader.IDE=CAN_ID_STD;
	  pHeader.RTR=CAN_RTR_DATA;
	  pHeader.StdId=Id;
	  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

		  if(HAL_CAN_AddTxMessage(&hcan1, &pHeader, mensaje, &TxMailbox3) != HAL_OK )
	  		{
	  				return HAL_ERROR;
	  		}
		  return HAL_OK;
	  }
	  return HAL_ERROR;
}





