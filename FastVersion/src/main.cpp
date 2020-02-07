#include "Arduino.h"
#include "EEPROM.h"

#include <stdio.h>

#include "ZPC_funcs.h"
#include "ZPC_IO.h"
#include "../program.h"

#define program program_CUSTOM

#define DEBUG_MODE (0)


#define IO_INT 0x04

char s[30];


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
  // while (!Serial.available())
  //   ;
  return Serial.available()?Serial.read():0;
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
//0x2045+0x00f9
//0x2045+0x015d
void ZPC_IO_Serial_PrintMemory(uint16_t from, uint16_t to){
  for (uint16_t i = from; i < to; i++)
  {
    uint8_t read_data = ZPC_MemRead(i);
    sprintf(s, "|%02x - %c|", read_data, read_data);
    // Serial.print(s);
    Serial.print(s);
  }
  Serial.print("\n");
  // for (uint16_t i = from; i < to; i++)
  // {
  //   uint8_t read_data = ZPC_MemRead(i);
  //   sprintf(s, "%02x ", read_data);
  //   // Serial.print(s);
  //   Serial.write(s);
  // }
}


void ZPC_ClockConfig()
{
  pinMode(CLK, OUTPUT);

  TCCR1A = ((1 << COM1A0));

  TCCR1B = ((1 << WGM12) | (1 << CS10));

  TIMSK1 = 0;

  OCR1A = 399;
}


void setup()
{
  Serial.begin(9600);
  Serial.print("init\n");

  ZPC_ArduinoInit();

  uint16_t address = 0x00;
  for (uint16_t i = 0; i < PROG_SIZE; i++)
  {
    ZPC_MemWrite(address + i, program[i]);
  }

  // for (uint16_t i = 0x2045+0x00f9; i < 0x2045+0x015d; i++)
  // {
  //   uint8_t read_data = ZPC_MemRead(address + i);
  //   //sprintf(s, "%02x ", read_data); //Should be equal to data
  //   Serial.write(read_data);
  // }
  ZPC_IO_Serial_PrintMemory(0x2045+0x00f9, 0x2045+0x015d);
  Serial.print("Start\n");

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
uint8_t M1 = 0;

uint16_t address = 0xffff;
uint8_t data = 0xff;
uint8_t int_vector = 0xff;

uint32_t start_time = millis();

void loop()
{
  if (Serial.available())
  {
    digitalWrite(INT_, LOW);
    int_vector = IO_INT; 
  }
  else
  {
    digitalWrite(INT_, HIGH);
  }

  IO = !digitalRead(WAIT_);
  W = !digitalRead(WR_);
  R = !digitalRead(RD_);
  M1 = !digitalRead(M1_);

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
    //sprintf(s, "Address : %04x Data: %02x \n", address, data);
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
    }
    else if (M1){          //Type 2 interrupt
      ZPC_DataSetOutput();
      ZPC_SetData(int_vector);
    }
    digitalWrite(BUSREQ_, LOW);
    digitalWrite(WAIT_RES_, LOW);
    delayMicroseconds(100);
    if (R|M1)
    {
      ZPC_DataSetInputPullup();
    }
    digitalWrite(WAIT_RES_, HIGH);
    digitalWrite(BUSREQ_, HIGH);
  }
}
