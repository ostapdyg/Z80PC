#ifndef ZPC_FUNCS_PROTO1
#define ZPC_FUNCS_PROTO1
#include "ZPC_pinout.h"
uint8_t __reverse(uint8_t reversee);

void ZPC_AddressSetOutput(void);

void ZPC_AddressSetInputPullup(void);

void ZPC_DataSetOutput(void);

void ZPC_DataSetInputPullup(void);

void ZPC_SetData(uint8_t data);

uint8_t ZPC_GetData();

void ZPC_SetAddress(uint16_t address);

uint16_t ZPC_GetAddress();

void ZPC_MemWrite(uint16_t address, uint8_t data);

uint8_t ZPC_MemRead(uint16_t address);

void ZPC_ArduinoInit(void);

#endif
