#ifndef CAN_DATA_ARRAY_H
#define CAN_DATA_ARRAY_H

#include <stdint.h>
#pragma pack(1)
typedef struct __attribute__((packed)) PackedStruct{
    uint32_t delay_ms;
    uint32_t id;
    uint8_t is_extended;
    uint8_t data[8];
} CAN_LogFrame;

extern const CAN_LogFrame can_data[];
extern uint32_t ARRAY_SIZE;
#endif
