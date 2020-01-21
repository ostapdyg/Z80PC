#include "Arduino.h"
#include "EEPROM.h"

#include <stdio.h>

#include "ZPC_funcs.h"
#include "ZPC_IO.h"
#include "../program.h"

#define program program_CUSTOM

#define DEBUG_MODE (0)

uint8_t program_TEMPLATE[] = {
    0x00, 0x00, 0x00, 0x00, //0x00
    0x00, 0x00, 0x00, 0x00, //0x04
    0x00, 0x00, 0x00, 0x00, //0x08
    0x00, 0x00, 0x00, 0x00, //0x0c
    0x00, 0x00, 0x00, 0x00, //0x10
    0x00, 0x00, 0x00, 0x00, //0x14
    0x00, 0x00, 0x00, 0x00, //0x18
    0x00, 0x00, 0x00, 0x00, //0x1c
    0x00, 0x00, 0x00, 0x00, //0x20
    0x00, 0x00, 0x00, 0x00, //0x24
    0x00, 0x00, 0x00, 0x00, //0x28
    0x00, 0x00, 0x00, 0x00, //0x2c
    0x00, 0x00, 0x00, 0x00, //0x30
    0x00, 0x00, 0x00, 0x00, //0x34
    0x00, 0x00, 0x00, 0x00, //0x38
    0x00, 0x00, 0x00, 0x00, //0x3c
    0x00, 0x00, 0x00, 0x00, //0x40
    0x00, 0x00, 0x00, 0x00, //0x44
    0x00, 0x00, 0x00, 0x00, //0x48
    0x00, 0x00, 0x00, 0x00, //0x4c
    0x00, 0x00, 0x00, 0x00, //0x50
    0x00};

uint8_t ZPC_IO_Serial_ReadByte()
{
  while (!Serial.available())
    ;
  return Serial.read();
}

uint8_t ZPC_IO_Serial_WriteByte(uint8_t data)
{
  Serial.write(data);
  return 0;
}

uint8_t ZPC_IO_ArduinoROM_ReadByte(uint16_t address)
{
  return EEPROM.read(address);
}

uint8_t ZPC_IO_ArduinoROM_WriteByte(uint16_t address, uint8_t data)
{
  EEPROM.write(address, data);
  return 0;
}

void ZPC_ClockConfig()
{
  pinMode(CLK, OUTPUT);

  TCCR1A = ((1 << COM1A0));

  TCCR1B = ((1 << WGM12) | (1 << CS10));

  TIMSK1 = 0;

  OCR1A = 399;
}

char s[30];

void setup()
{
  Serial.begin(9600);
  Serial.print("start\n");

  ZPC_ArduinoInit();

  uint16_t address = 0x00;
  for (uint16_t i = 0; i < PROG_SIZE; i++)
  {
    ZPC_MemWrite(address + i, program[i]);
  }

  // for (uint16_t i = 0; i < PROG_SIZE; i++)
  // {
  //   uint8_t read_data = ZPC_MemRead(address + i);
  //   sprintf(s, "%02x ", read_data); //Should be equal to data
  //   Serial.print(s);
  // }

  pinMode(CLK, OUTPUT);

  pinMode(13, OUTPUT);
  pinMode(INT_, OUTPUT);
  digitalWrite(INT_, 1);
  ZPC_ClockConfig();

  ZPC_ProcStart();
  digitalWrite(RESET_, LOW);
  delay(1);
  digitalWrite(RESET_, HIGH);
}

uint8_t W = 0;
uint8_t R = 0;
uint8_t IO = 0;

uint16_t address = 0xffff;
uint8_t data = 0xff;

uint32_t start_time = millis();

void loop()
{
  if (Serial.available())
  {

    digitalWrite(INT_, 0);
    delay(1);
    digitalWrite(INT_, 1);
  }

  IO = !digitalRead(WAIT_);
  W = !digitalRead(WR_);
  R = !digitalRead(RD_);

  address = ZPC_GetAddress();
  data = ZPC_GetData();

  if (DEBUG_MODE)
  {
    if (IO)
    {
      Serial.print("IO ");
    }
    if (W)
    {
      Serial.print("Write ");
    }
    if (R)
    {
      Serial.print("Read ");
    }
    sprintf(s, "Address : %04x Data: %02x \n", address, data);
    Serial.print(s);
  }

  if (IO)
  {
    sprintf(s, "Address : %04x Data: %02x \n", address, data);
    if (R)
    {
      // First tact of input cycle
      uint8_t data_in = 0xff;
      uint8_t port = address & 0xff;

      switch (port)
      {
      case 0x0a:
        data_in = ZPC_IO_ArduinoROM_ReadByte(address);
        break;
      case 0x01:
        data_in = ZPC_IO_Serial_ReadByte();
        break;
      default:
        data_in = ZPC_IO_Serial_ReadByte();
      }
      ZPC_DataSetOutput();
      ZPC_SetData(data_in);
      digitalWrite(BUSREQ_, LOW);
      digitalWrite(WAIT_RES_, LOW);
      delayMicroseconds(2);
      ZPC_DataSetInputPullup();
      digitalWrite(WAIT_RES_, HIGH);
      digitalWrite(BUSREQ_, HIGH);
    }
    else if (W)
    {
      uint8_t port = address & 0xff;
      switch (port)
      {
      case 0x0a:
        ZPC_IO_ArduinoROM_WriteByte(address, data);
        break;
      case 0x01:
        ZPC_IO_Serial_WriteByte(data);
        break;
      case 0x0f:
        sprintf(s, "Running time: %li ms \n", millis() - start_time);
        Serial.print(s);
        start_time = millis();
        break;
      default:
        ZPC_IO_Serial_WriteByte(data);
      }
      digitalWrite(BUSREQ_, LOW);
      digitalWrite(WAIT_RES_, LOW);
      delayMicroseconds(2);
      digitalWrite(WAIT_RES_, HIGH);
      digitalWrite(BUSREQ_, HIGH);
    }
  }

  if (Serial.available())
  {
    digitalWrite(INT_, LOW);
    delayMicroseconds(2);
    digitalWrite(INT_, HIGH);
  }
}