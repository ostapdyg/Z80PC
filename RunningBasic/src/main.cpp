#include "Arduino.h"
#include "EEPROM.h"
#include <PS2Keyboard.h>

#include <stdio.h>

#include "ZPC_funcs.h"
#include "ZPC_IO.h"
#include "../program.h"

#define program program_CUSTOM

#define DEBUG_MODE (0) //Possible values: R|W or IO

#define SERIAL_INTERRUPT_VECTOR 0x12

#define KEYBOARD_DATA 2
#define KEYBOARD_IRQ 3

PS2Keyboard keyboard;

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
  while (!keyboard.available())
    ;
  return keyboard.read();
}

uint8_t ZPC_IO_Serial_WriteByte(uint8_t data)
{
  Serial1.write(data);
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

void setup()
{

  char s[30];
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial.print("start\n");

  keyboard.begin(KEYBOARD_DATA, KEYBOARD_IRQ);

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
  //   Serial1.print(s);
  // }
  // Serial1.print("\n");

  pinMode(CLK, OUTPUT);
  ZPC_ProcStart();
  //Reset 1-0-1 pulse for at least 4 CLK ticks
  digitalWrite(RESET_, LOW);
  for (uint8_t i = 0; i <= 10; i++)
  {
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
  digitalWrite(RESET_, HIGH);
}

char s[30];
uint32_t i = 0;
uint8_t W = 0;
uint8_t R = 0;
uint8_t IO = 0;

uint8_t IO_CYCLE_ACTIVE = 0;
uint8_t PINS_OUTPUT = 0;
uint8_t INTERRUPT_ACTIVE = 0;

uint16_t address = 0xffff;
uint8_t data = 0xff;
uint8_t interrupt_vector = 0xff;

uint32_t start_time = millis();

void loop()
{
  digitalWrite(CLK, LOW);
  digitalWrite(CLK, HIGH);
  if (INTERRUPT_ACTIVE)
  {
    // if(!Serial1.available()){
    INTERRUPT_ACTIVE = 0;
    // }
    digitalWrite(INT_, HIGH);
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
    if (INTERRUPT_ACTIVE)
    {
      Serial.print("Int ");
    }
    sprintf(s, "Address : %04x Data: %02x \n", address, data);
    Serial.print(s);
  }

  if ((IO_CYCLE_ACTIVE) && !(IO) && !(R | W))
  {
    if (PINS_OUTPUT)
    {
      ZPC_DataSetInputPullup();
      PINS_OUTPUT = 0;
    }
    IO_CYCLE_ACTIVE = 0;
    digitalWrite(WAIT_RES_, HIGH);
  }

  if (IO && !IO_CYCLE_ACTIVE)
  {
    if (R)
    {
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
      PINS_OUTPUT = 1;
      ZPC_DataSetOutput();
      ZPC_SetData(data_in);
    }
    else if (W)
    {
      uint8_t data_in = 0xff;
      uint8_t port = address & 0xff; //4 oldest bits in address to choose io type
      switch (port)
      {
      case 0x0a:
        ZPC_IO_ArduinoROM_WriteByte(address, data);
        break;
      case 0x01:
        ZPC_IO_Serial_WriteByte(data);
        break;
      default:
        ZPC_IO_Serial_WriteByte(data);
      }
    }
    IO_CYCLE_ACTIVE = 1;
    digitalWrite(WAIT_RES_, LOW);
  }

  if (keyboard.available() && !INTERRUPT_ACTIVE)
  {
    INTERRUPT_ACTIVE = 1;

    digitalWrite(INT_, LOW);
  }
}