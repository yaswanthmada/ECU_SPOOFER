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
uint32_t TOTAL_MESSAGES=(sizeof(can_data)/sizeof(can_data[0]));

void can_gpio_init(){
	RCC->APB1ENR|=RCC_APB1ENR_CAN1EN ;
	RCC->APB2ENR|=RCC_APB2ENR_AFIOEN;
	AFIO->MAPR|=AFIO_MAPR_CAN_REMAP_REMAP2;
	RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
	GPIOB->CRH|=(1<<2);
	GPIOB->CRH|=(0xb0);
}
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
    // 500 kbps with 8 MHz: 8
    CAN1->BTR = 0x00050001;//(6 << 20) | (1 << 16) | (0 << 0); // TS1=6, TS2=1, Prescaler=1
    // Leave init mode
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK);
}
void CAN_Send(const uint32_t id,const  uint8_t is_std_etd,const uint8_t *data) {

    if((CAN1->TSR & CAN_TSR_TME0)!=0){ // Wait for empty mailbox
    	CAN1->sTxMailBox[0].TIR&=~(0xffffffff);
    	if(is_std_etd==0){
    		CAN1->sTxMailBox[0].TIR |= (id <<21);       // Standard ID
    	}else{
    		CAN1->sTxMailBox[0].TIR |=(1<<2);
    		CAN1->sTxMailBox[0].TIR |=(id<<3);
    	}
        CAN1->sTxMailBox[0].TDTR = 8;               // Data length
        CAN1->sTxMailBox[0].TDLR=(((uint32_t)data[3]<<24)|((uint32_t)data[2]<<16)|((uint32_t)data[1]<<8)|((uint32_t)data[0]<<0));
        CAN1->sTxMailBox[0].TDHR=(((uint32_t)data[7]<<24)|((uint32_t)data[6]<<16)|((uint32_t)data[5]<<8)|((uint32_t)data[4]<<0));
        CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;   // Request transmission
    } else if((CAN1->TSR & CAN_TSR_TME1)!=0){ // Wait for empty mailbox
    	CAN1->sTxMailBox[1].TIR&=~(0xffffffff);
    	if(is_std_etd==0){
    		CAN1->sTxMailBox[1].TIR |= (id <<21);       // Standard ID
    	}else{
    		CAN1->sTxMailBox[1].TIR |=(1<<2);
    		CAN1->sTxMailBox[1].TIR |=(id<<3);
    	}
        CAN1->sTxMailBox[1].TDTR = 8;               // Data length
        CAN1->sTxMailBox[1].TDLR=(((uint32_t)data[3]<<24)|((uint32_t)data[2]<<16)|((uint32_t)data[1]<<8)|((uint32_t)data[0]<<0));
        CAN1->sTxMailBox[1].TDHR=(((uint32_t)data[7]<<24)|((uint32_t)data[6]<<16)|((uint32_t)data[5]<<8)|((uint32_t)data[4]<<0));
        CAN1->sTxMailBox[1].TIR |= CAN_TI0R_TXRQ;   // Request transmission
    }else if((CAN1->TSR & CAN_TSR_TME2)!=0){ // Wait for empty mailbox
    	CAN1->sTxMailBox[2].TIR&=~(0xffffffff);
    	if(is_std_etd==0){
    		CAN1->sTxMailBox[2].TIR |= (id <<21);       // Standard ID
    	}else{
    		CAN1->sTxMailBox[2].TIR |=(1<<2);
    		CAN1->sTxMailBox[2].TIR |=(id<<3);
    	}
        CAN1->sTxMailBox[2].TDTR = 8;               // Data length
        CAN1->sTxMailBox[2].TDLR=(((uint32_t)data[3]<<24)|((uint32_t)data[2]<<16)|((uint32_t)data[1]<<8)|((uint32_t)data[0]<<0));
        CAN1->sTxMailBox[2].TDHR=(((uint32_t)data[7]<<24)|((uint32_t)data[6]<<16)|((uint32_t)data[5]<<8)|((uint32_t)data[4]<<0));
        CAN1->sTxMailBox[2].TIR |= CAN_TI0R_TXRQ;   // Request transmission
    }



}
void ECU_SPOOFER_PROCESS(){
	    if (msg_index >= TOTAL_MESSAGES)
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
	  for(uint32_t i=0;i<(TOTAL_MESSAGES);i++){
			  CAN_Send(can_data[i].id,can_data[i].is_extended, can_data[i].data);
      }
}
void CAN_TransmitMessage()
{
    if((CAN1->TSR & CAN_TSR_TME0)!=0) // Wait for empty mailbox
    {
    CAN1->sTxMailBox[0].TIR = (123 << 3)|(1<<2);       // Standard ID
    CAN1->sTxMailBox[0].TDTR = 1;               // Data length
    CAN1->sTxMailBox[0].TDLR = 0xab;
    CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;   // Request transmission
    }
//	if((((CAN1->TSR>>26)&1) !=0)){
//	    uint32_t check_mailbox_free=0;//(CAN1->TSR>>24)&0X3;
//	    CAN1->sTxMailBox[check_mailbox_free].TIR=0;
//	    CAN1->sTxMailBox[check_mailbox_free].TIR&=~(1<<2);
//	    CAN1->sTxMailBox[check_mailbox_free].TIR&=~(1<<1);
//        CAN1->sTxMailBox[check_mailbox_free].TIR|=(txmessage->id<<21);
//        CAN1->sTxMailBox[check_mailbox_free].TDTR=txmessage->dlc;
//        CAN1->sTxMailBox[check_mailbox_free].TDLR=(((uint32_t)txmessage->data[3]<<24)|((uint32_t)txmessage->data[2]<<16)|((uint32_t)txmessage->data[1]<<8)|((uint32_t)txmessage->data[0]<<0));
//        CAN1->sTxMailBox[check_mailbox_free].TDHR=(((uint32_t)txmessage->data[7]<<24)|((uint32_t)txmessage->data[6]<<16)|((uint32_t)txmessage->data[5]<<8)|((uint32_t)txmessage->data[4]<<0));
//        CAN1->sTxMailBox[check_mailbox_free].TIR=(1<<0);
//	}
//	        uint32_t tsr = READ_REG(CAN1->TSR);
//		     uint32_t transmitmailbox;
//		    /* Check that all the Tx mailboxes are not full */
//		    if(((tsr & CAN_TSR_TME0) != 0U)||((tsr & CAN_TSR_TME1) != 0U)||((tsr & CAN_TSR_TME1) != 0U) ||((tsr & CAN_TSR_TME2) != 0U))
//		    {
//		    	/* Select an empty transmit mailbox */
//		    	transmitmailbox = (tsr & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;
//	            CAN1->sTxMailBox[transmitmailbox].TIR=0x123<<21;
//	            CAN1->sTxMailBox[transmitmailbox].TDTR=8;
//	            /* Set up the data field */
//	                 WRITE_REG(CAN1->sTxMailBox[transmitmailbox].TDHR,
//	                           ((uint32_t)'a' << CAN_TDH0R_DATA7_Pos) |
//	                           ((uint32_t)'n' << CAN_TDH0R_DATA6_Pos) |
//	                           ((uint32_t)0xa << CAN_TDH0R_DATA5_Pos) |
//	                           ((uint32_t)4 << CAN_TDH0R_DATA4_Pos));
//	                 WRITE_REG(CAN1->sTxMailBox[transmitmailbox].TDLR,
//	                           ((uint32_t)'a' << CAN_TDL0R_DATA3_Pos) |
//	                           ((uint32_t)2 << CAN_TDL0R_DATA2_Pos) |
//	                           ((uint32_t)1 << CAN_TDL0R_DATA1_Pos) |
//	                           ((uint32_t)0 << CAN_TDL0R_DATA0_Pos));
//	             /* Request transmission */
//	             SET_BIT(CAN1->sTxMailBox[transmitmailbox].TIR, CAN_TI0R_TXRQ);
//
//		    }

}

