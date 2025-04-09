/*
 * can.h
 *
 *  Created on: Mar 20, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_
#include"DEFINES.h"
void can_gpio_init();
void CAN_Init();
int can_tx_message(CAN_TxMessage_t *);
void CAN_TransmitMessage();
void CAN_Send(const uint32_t id,const uint8_t,const uint8_t *data);
void send();
void ECU_SPOOFER_PROCESS();


#endif /* INC_CAN_H_ */
