#include "Arduino.h"

#include <stdio.h>

#include "ZPC_funcs.h"
#include "ZPC_IO_funcs.h"
#include "ZPC_visualisation.h"
#include "../program.h"

#define program program_CUSTOM

#define DEBUG_MODE (0)

#define IO_INT 0x04

char s[30];

static PCD8544 lcd(A0, A1, A2, A4, A3);
static ZPC_Displayer displayer(lcd);

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

// uint8_t ZPC_IO_Serial_ReadByte()
// {
//   // while (!Serial.available())
//   //   ;
//   return Serial.available() ? Serial.read() : 0;
// }

// uint8_t ZPC_IO_Serial_WriteByte(uint8_t data)
// {
//   Serial.write(data);
//   return 0;
// }

// uint8_t ZPC_IO_ArduinoROM_ReadByte(uint16_t address)
// {
//   return EEPROM.read(address);
// }

// uint8_t ZPC_IO_ArduinoROM_WriteByte(uint16_t address, uint8_t data)
// {
//   EEPROM.write(address, data);
//   return 0;
// }

// //0x2045+0x00f9
// //0x2045+0x015d

// void ZPC_IO_Serial_PrintMemory(uint16_t from, uint16_t to)
// {
//   for (uint16_t i = from; i < to; i++)
//   {
//     uint8_t read_data = ZPC_MemRead(i);
//     sprintf(s, "|%02x - %c|", read_data, read_data);
//     // Serial.print(s);
//     Serial.print(s);
//   }
//   Serial.print("\n");
//   // for (uint16_t i = from; i < to; i++)
//   // {
//   //   uint8_t read_data = ZPC_MemRead(i);
//   //   sprintf(s, "%02x ", read_data);
//   //   // Serial.print(s);
//   //   Serial.write(s);
//   // }
// }

void ZPC_ClockConfig()
{
  pinMode(CLK, OUTPUT);

  TCCR1A = ((1 << COM1A0));

  TCCR1B = ((1 << WGM12) | (1 << CS10));

  TIMSK1 = 0;

  OCR1A = 399;
}

// void ZPC_ReadRAM(uint8_t *dest, uint16_t from, uint16_t size)
// {
//   for (uint16_t p = 0; p < size; p++)
//   {
//     *(dest + p) = ZPC_MemRead(from + p);
//   }
// }

// void ZPC_WriteRAM(uint16_t dest, uint8_t *src, uint16_t size)
// {
//   for (uint16_t p = 0; p < size; p++)
//   {
//     ZPC_MemWrite(dest + p, *(src + p));
//   }
// }
inline void ZPC_DisplayRAM(ZPC_Displayer* displayer){
  ZPC_MemReadBlock(displayer->ram_data, displayer->ram_addr, displayer->ram_size);
  displayer->refresh();
}

void ZPC_IO_HandleWrite(uint16_t address, uint8_t data)
{
  uint8_t port = address & 0xff;
  // ZPC_IO_Serial_WriteByte(data);
  switch (port)
  {
  case 0x00:  // Wait till a key is pressed
    sprintf(s, "Press any key to continue:\n");
    Serial.print(s);
    while(!Serial.available());
    Serial.read();
    break;
  case 0x0a:  // Save byte to ROM
    ZPC_IO_ArduinoROM_WriteByte(address, data);
    break;
  case 0x01:  // Print byte as char
    ZPC_IO_Serial_WriteByte(data);
    break;
  case 0x02:  // Print byte as hex
    sprintf(s, "%02X", data);
    Serial.print(s);
    break;
  case 0x0f:
    static uint32_t running_time;
    sprintf(s, "Running time: %li ms \n", millis() - running_time);
    Serial.print(s);
    running_time = millis();
    break;
  case 0x30:  //Debug output: A
  case 0x31:  //Debug output: F
  case 0x32:  //Debug output: B
  case 0x33:  //Debug output: C
  case 0x34:  //Debug output: D
  case 0x35:  //Debug output: E
  case 0x36:  //Debug output: H
  case 0x37:  //Debug output: L
  case 0x38:  //Debug output: IXh
  case 0x39:  //Debug output: IXl
  case 0x3a:  //Debug output: IYh
  case 0x3b:  //Debug output: IYl
  case 0x3c:  //Debug output: SPh
  case 0x3d:  //Debug output: SPl
  case 0x3e:  //Debug output: PCh
  case 0x3f:  //Debug output: PCl
    // sprintf(s, "Debug: %X - %02X\n", port&0xf, data);
    // Serial.print(s);
    displayer.set_register(port - 0x30, data);
    break;
  case 0x40:  //Debug output: refresh displayer
    displayer.refresh();
    break;
  default:
    ZPC_IO_Serial_WriteByte(data);
  }
};

void ZPC_IO_HandleRead(uint8_t address)
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
  ZPC_DataSetOutput();
  ZPC_SetData(data_in);
}

void setup()
{
  Serial.begin(9600);
  Serial.print("init\n");

  displayer.begin();

  ZPC_ArduinoInit();

  // uint16_t address = 0x00;
  // for (uint16_t i = 0; i < PROG_SIZE; i++)
  // {
  //   ZPC_MemWrite(address + i, program[i]);
  // }
  ZPC_MemWriteBlock(0, program, PROG_SIZE);

  // ZPC_MemReadBlock(displayer.ram_data, displayer.ram_addr, displayer.ram_size);
  // displayer.refresh();
  displayer.set_register(rA, 0xa);
  ZPC_DisplayRAM(&displayer);

  // ZPC_

  // for(uint16_t i = 0; i < disp.ram_size)

  // for (uint16_t i = 0; i < 28; i++)
  // {
  //   uint8_t read_data = ZPC_MemRead(i);
  //   sprintf(s, "%02x ", read_data); //Should be equal to data
  //   Serial.print(s);
  // }

  // ZPC_IO_Serial_PrintMemory(0, 28);
  Serial.print("Start\n");

  pinMode(USER_LED, OUTPUT);
  // pinMode(INT_, OUTPUT);    //!!
  // digitalWrite(INT_, HIGH); //!!
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


void loop()
{

  // TODO: ugly, improve
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
      ZPC_IO_HandleRead(address);
    }
    else if (W)
    {
      ZPC_IO_HandleWrite(address, data);
    }
    else if (M1)
    { //Type 2 interrupt
      ZPC_DataSetOutput();
      ZPC_SetData(int_vector);
    }
    digitalWrite(BUSREQ_, LOW);
    digitalWrite(WAIT_RES_, LOW);
    delayMicroseconds(100);
    if (R | M1)
    {
      ZPC_DataSetInputPullup();
    }
    digitalWrite(WAIT_RES_, HIGH);
    digitalWrite(BUSREQ_, HIGH);
  }
}
