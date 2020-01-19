#include "Arduino.h"
#include "EEPROM.h"

#include <stdio.h>

#include "ZPC_funcs.h"
#include "ZPC_IO.h"
#include "../z80_prog/program.h"
#include "PS2Keyboard.h"

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

  uint8_t program_SORT[] = {
      0xc3, 0x0c, 0x00, 0x00, //0x00  JP  (0x0c,0x00)
      0x02, 0x03, 0x01, 0x06, //0x04  Arr
      0x05, 0x02, 0x04, 0x08, //0x08  Arr
      0x00, 0x00, 0x00, 0x00, //0x0c  Local vars
      0x2a, 0x02, 0x00, 0x00, //0x10  LD   HL, (0x02, 0x00) ;Arr_end
      0xdd, 0x2a, 0x04, 0x00, //0x14  LD   IX,  0x04, 0x00  ;First index
      0xdd, 0xe5, 0xfd, 0xe1, //0x18  PUSH IX, POP IY       ;Second index = first index
      0xfd, 0x23, 0x00, 0x00, //0x1c  INC  IY
      0x0a, 0x00, 0x00, 0x00, //0x20  LD
      0x00, 0x00, 0x00, 0x00, //0x24
      0x00, 0x00, 0x00, 0x00, //0x28  JR   NC,  0x03c        ;If !(DE)<(BC)
      0xed, 0x43, 0x3a, 0x00, //0x2c  LD  (0x3a, 0x00), BC
      0xed, 0x53, 0x36, 0x00, //0x30  LD  (0x36, 0x00), DE
      0xed, 0x4b, 0x00, 0x00, //0x34  LD   BC,  0x00, 0x00
      0xed, 0x5b, 0x00, 0x00, //0x38  LD   DE,  0x00, 0x00
      0x00, 0x00, 0x00, 0x00, //0x3c
      0x00, 0x00, 0x00, 0x00, //0x40
      0x00, 0x00, 0x00, 0x00, //0x44
      0x00, 0x00, 0x00, 0x00, //0x48
      0x00, 0x00, 0x00, 0x00, //0x4c
      0x00, 0x00, 0x00, 0x00, //0x50
      0x00};

  uint8_t program_ITER[] = {
      0xc3, 0x0c, 0x00, 0x00, //0x00  JP  (0x10, 0x00)
      0x02, 0x03, 0x01, 0x06, //0x04  Arr
      0x05, 0x02, 0x04, 0x08, //0x08  Arr
      0x3a, 0x01, 0x00, 0x00, //0x0c  LD   A,  (0x01, 0x00)            ;Size
      0x00, 0x00, 0xd6, 0x04, //0x10  LD   SUB 0x08
      0xdd, 0x21, 0x04, 0x00, //0x14  LD   IX,  0x04, 0x00             ;Arr start
      0xdd, 0x46, 0x00, 0x70, //0x18  LD   B,  (IX + 0), PUSH BC       ;IO imitation
      0xdd, 0x23, 0x00, 0x00, //0x1c  INC  IX,  PUSH AF
      0xd6, 0x01, 0x20, 0xf4, //0x20  SUB  0x01,  JR NZ, 0xf6
      0x76, 0x00, 0x00, 0x00, //0x24  HALT                             ;End program
    };

  uint8_t program_IO[] = {
      0x3e, 0x0e,
      0xd3, 0x12, 0x00, 0x00};

  // uint8_t program_CUSTOM[] = {
  //   /*%!! array !!%*/
// };



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

void setup()
{

  char s[30];
  Serial.begin(9600);
  Serial.print("start\n");

  ZPC_ArduinoInit();

  uint16_t address = 0x00;
  for (uint16_t i = 0; i < PROG_SIZE; i++)
  {
    ZPC_MemWrite(address + i, program[i]);
  }

  for (uint16_t i = 0; i < PROG_SIZE; i++)
  {
    uint8_t read_data = ZPC_MemRead(address + i);
    sprintf(s, "%02x ", read_data); //Should be equal to data
    Serial.print(s);
  }
  pinMode(CLK, OUTPUT);
  Serial.print("\n");

  ZPC_ProcStart();
}
char s[30];
uint32_t i = 0;
uint8_t W = 0;
uint8_t R = 0;
uint8_t IO = 0;

uint8_t IO_CYCLE_ACTIVE = 0;
uint8_t PINS_OUTPUT = 0;

uint16_t address = 0xffff;
uint8_t data = 0xff;

uint32_t start_time = millis();

void loop()
{
  digitalWrite(CLK, LOW);
  digitalWrite(CLK, HIGH);

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

  if (IO && !IO_CYCLE_ACTIVE)
  {
    if (R)
    {
      // First tact of input cycle
      uint8_t data_in = 0xff;
      uint8_t io_type = address >> 12; //4 oldest bits in address to choose io type
      switch (io_type)
      {
      case 0x0:
        data_in = ZPC_IO_ArduinoROM_ReadByte(address);
        //Serial.print("ROM read");
        break;
      case 0x1:
        data_in = ZPC_IO_Serial_ReadByte();
        break;
      default:
        Serial.print("Invalid IO address\nShutting down...\n");
        while (1)
          ;
      }
      PINS_OUTPUT = 1;
      ZPC_DataSetOutput();
      ZPC_SetData(data_in);
    }
    else if (W)
    {
      uint8_t io_type = address >> 12; //4 oldest bits in address to choose io type
      switch (io_type)
      {
      case 0b0000:
        ZPC_IO_ArduinoROM_WriteByte(address, data);
        break;
      case 0b0001:
        //Serial.print("ROM write");
        ZPC_IO_Serial_WriteByte(data);
        break;
      case 0xf:
        sprintf(s, "Running time: %li", millis() - start_time);
        Serial.print(s);
        start_time = millis();
        break;
      default:
        Serial.print("Invalid IO address\nShutting down...\n");
        while (1)
          ;
      }
    }
    IO_CYCLE_ACTIVE = 1;
    digitalWrite(WAIT_RES_, LOW);
  }
  if (IO_CYCLE_ACTIVE && !(W || R))
  {
    if (PINS_OUTPUT)
    {
      ZPC_DataSetInputPullup();
      PINS_OUTPUT = 0;
    }
    IO_CYCLE_ACTIVE = 0;
    digitalWrite(WAIT_RES_, HIGH);
  }

  //delay(100);
}