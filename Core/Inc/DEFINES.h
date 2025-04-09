/*
 * DEFINES.h
 *
 *  Created on: Mar 20, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_DEFINES_H_
#define INC_DEFINES_H_
#include<stdint.h>
#define S_BIT(WORD,BIT_POS) WORD|=(1<<BIT_POS)
#define C_BIT(WORD,BIT_POS) WORD&=~(1<<BIT_POS)
#define GET_BIT(WORD,BIT_POS) ((WORD>>BIT_POS)&1)
#define TOGGLE_BIT(WORD,BIT_POS) WORD^=(1<<BIT_POS)
#define SET_BYTE(WORD, BYTE, VALUE)  ((WORD) = ((WORD) & ~(0xFFU << ((BYTE) * 8))) | ((VALUE) << ((BYTE) * 8)))


#define CAN_TIMEOUT_VALUE 10U

typedef enum
{
	FALSE,
	TRUE
}STATUS;

typedef struct {
    uint32_t id ;   // CAN Identifier (Standard: 11 bits, Extended: 29 bits)
    uint32_t ide ;    // Identifier Extension (0 for Standard, 1 for Extended)
    uint32_t rtr ;    // Remote Transmission Request (0 for Data, 1 for Remote)
    uint32_t dlc ;    // Data Length Code (0-8 bytes)
    uint8_t data[8];    // Data payload
} CAN_TxMessage_t;
#endif /* INC_DEFINES_H_ */
