#ifndef ZPC_IO_FUNCS_PROTO1
#define ZPC_IO_FUNCS_PROTO1
#include "Arduino.h"
#include "EEPROM.h"

#include "ZPC_pinout.h"
#include "ZPC_funcs.h"



// void ZPC_IO_HandleWrite(uint16_t address, uint8_t data);

// void ZPC_IO_HandleRead(uint8_t address);

uint8_t ZPC_IO_Serial_ReadByte();

uint8_t ZPC_IO_Serial_WriteByte(uint8_t data);

uint8_t ZPC_IO_ArduinoROM_ReadByte(uint16_t address);

uint8_t ZPC_IO_ArduinoROM_WriteByte(uint16_t address, uint8_t data);

// void ZPC_IO_Serial_PrintMemory(uint16_t from, uint16_t to);

#endif