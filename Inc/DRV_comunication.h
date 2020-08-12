/*
 * DRV_CAN.h
 *
 *  Created on: 4 mar. 2020
 *      Author: Manuel
 */

#ifndef DRV_CAN_H_
#define DRV_CAN_H_
/*Private includes*/

#include "main.h"
#include "board.h"

/*Private Function*/
int can1_Tx(uint16_t Id ,uint8_t mensaje [],uint8_t bytes);

/*extern functions*/
extern void Error_Handler(void);



#endif /* DRV_CAN_H_ */
