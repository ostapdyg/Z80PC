#ifndef ZPC_FUNCS_PROTO1
#define ZPC_FUNCS_PROTO1
#include "Arduino.h"
#include "ZPC_pinout.h"


//Return a reversed binary data
uint16_t __reverse(uint16_t reversee, uint8_t bitcount);

//Return a reversed uint8
#define __reverse8(x) __reverse(x, 8)

//Return a reversed uint16
#define __reverse16(x) __reverse(x, 16)

//Set Arduino address pins to output
void ZPC_AddressSetOutput(void);

//Set Arduino address pins to input with pullup
void ZPC_AddressSetInputPullup(void);

//Set Arduino data pins to output
void ZPC_DataSetOutput(void);

//Set Arduino data pins to input with pullup
void ZPC_DataSetInputPullup(void);

void ZPC_TakeBus(void);
void ZPC_FreeBus(void);

//Write a byte to Arduino data pins
void ZPC_SetData(uint8_t data);

//Read a byte from Arduino data pins
uint8_t ZPC_GetData();

//Write two bytes to Arduino address pins
void ZPC_SetAddress(uint16_t address);

//Read two bytes from Arduino address pins
uint16_t ZPC_GetAddress();

//Write aone byte to RAM
void ZPC_MemWrite(uint16_t address, uint8_t data);

// Copy data from src in Arduino to dest in RAM
void ZPC_MemWriteBlock(uint16_t dest, uint8_t *src, uint16_t size);

//Read one byte from RAM
uint8_t ZPC_MemRead(uint16_t address);

// Copy data from src in RAM to dest in Arduino
void ZPC_MemReadBlock(uint8_t *dest, uint16_t src, uint16_t size);


//Initialize Z80 control pins
void ZPC_ArduinoInit(void);


//Set data and addresses to intput and start the processor
void ZPC_ProcStart(void);

#endif
