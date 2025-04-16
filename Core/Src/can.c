/*
 * can.c
 *
 *  Created on: Mar 20, 2025
 *      Author: MadaYaswanth
 */

#include"DEFINES.h"
#include"main.h"
#include"stm32f103xb.h"
#include"stm32f1xx.h"
#include"can_data_array1.h"
#include<stdio.h>
#include"can.h"


extern void uart_tx(uint8_t c);
extern void uart_int(int c);
extern void uart_string(char *);
uint32_t lastSendTime = 0;
uint32_t msg_index = 0;

void CAN_Init(void) {
    // Enable GPIOB, AFIO, CAN clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    // Remap CAN1 to PB8 (RX), PB9 (TX)
    AFIO->MAPR |= AFIO_MAPR_CAN_REMAP_REMAP2; // Full remap (PB8/PB9)
    // PB8 = input floating (CAN RX)
    GPIOB->CRH &= ~(0xF << 0); // Clear bits for PB8
    GPIOB->CRH |=  (0x4 << 0); // Input floating (0b0100)
    // PB9 = Alternate Function Push-Pull (CAN TX, 50 MHz)
    GPIOB->CRH &= ~(0xF << 4); // Clear bits for PB9
    GPIOB->CRH |=  (0xB << 4); // AF PP output (0b1011)
     /* Exit from sleep mode */
	CLEAR_BIT(CAN1->MCR, CAN_MCR_SLEEP);
	 /* Check Sleep mode leave acknowledge */
	 while ((CAN1->MSR & CAN_MSR_SLAK) != 0U){
	   }
    // CAN in init mode
    CAN1->MCR |= CAN_MCR_INRQ;
    while (!(CAN1->MSR & CAN_MSR_INAK));
    // 500 kbps with 8 MHz: 8// modified to 36MHZ
    CAN1->BTR =0x001e0003;
    // Leave init mode
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);
}
uint8_t CAN_Send(const uint32_t id,const  uint8_t is_std_etd,const uint8_t *data) {
     uint8_t mail_box;
	if((CAN1->TSR & CAN_TSR_TME0)!=0){
		mail_box=0;
	}else if((CAN1->TSR & CAN_TSR_TME1)!=0){
		mail_box=1;
	}else if((CAN1->TSR & CAN_TSR_TME2)!=0){
		mail_box=2;
	}else{
		return 0;
	}
	CAN1->sTxMailBox[mail_box].TIR&=~(0xffffffff);
	if(is_std_etd==0){// Standard ID
		CAN1->sTxMailBox[mail_box].TIR |= (id <<21);
	}else{//Extended id
		CAN1->sTxMailBox[mail_box].TIR |=(1<<2);
		CAN1->sTxMailBox[mail_box].TIR |=(id<<3);
	}
    CAN1->sTxMailBox[mail_box].TDTR = 8;               // Data length
    CAN1->sTxMailBox[mail_box].TDLR=(((uint32_t)data[3]<<24)|((uint32_t)data[2]<<16)|((uint32_t)data[1]<<8)|((uint32_t)data[0]<<0));
    CAN1->sTxMailBox[mail_box].TDHR=(((uint32_t)data[7]<<24)|((uint32_t)data[6]<<16)|((uint32_t)data[5]<<8)|((uint32_t)data[4]<<0));
    CAN1->sTxMailBox[mail_box].TIR |= CAN_TI0R_TXRQ;   // Request transmission
    return 1;
}
void ECU_SPOOFER_PROCESS(){
	    if (msg_index >= ARRAY_SIZE)
	    {
	    	msg_index=0;
	    }
        if ((HAL_GetTick() - lastSendTime)>=(can_data[msg_index].delay_ms)) {
        	lastSendTime=HAL_GetTick();
            CAN_Send(can_data[msg_index].id,can_data[msg_index].is_extended,can_data[msg_index].data);
            msg_index++;
	    }
}
void send(){
	 uint8_t k;
	 static int i=0;
	 k=(CAN_Send(can_data[i].id,can_data[i].is_extended, can_data[i].data)!=0);
	 if(k==1){
		 i++;
	 }
	 if(i==ARRAY_SIZE){
		 i=0;
	 }
}
