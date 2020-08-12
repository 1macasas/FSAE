/*
 * DRV_CAN.c
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */

/*includes*/
#include <DRV_comunication.h>

/*Private Variables*/
 CAN_TxHeaderTypeDef pHeader;


/*Extern Variables*/
extern CAN_HandleTypeDef hcan1;
extern  uint32_t TxMailbox;

// mi driver para enviar info por can
// le doy el ID y el mensaje a enviar, y me devuelve un 1 si se envio correctamente, o un cero si tuvo algun error

int can1_Tx(uint16_t Id ,uint8_t mensaje [],uint8_t bytes)
{

	  if(HAL_CAN_IsTxMessagePending(&hcan1, CAN_TX_MAILBOX0) == 0)
	  {
	  pHeader.DLC=bytes;
	  pHeader.IDE=CAN_ID_STD;
	  pHeader.RTR=CAN_RTR_DATA;
	  pHeader.StdId=Id;
	  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

		  if(HAL_CAN_AddTxMessage(&hcan1, &pHeader, mensaje, &TxMailbox) != HAL_OK )
	  		{
	  				return HAL_ERROR;
	  		}
		  return HAL_OK;
	  }
	  return HAL_ERROR;
}


