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



// -----------------------------------------------------------------Clock Manipulation----------------------------------------------------------------------------------
void ZPC_ClockConfig()
{
  pinMode(CLK, OUTPUT);

  TCCR1A = (1 << COM1A0);

  TCCR1B = (1 << WGM12); //Set timer mode to CTC but do not start the timer.

  TIMSK1 = 0;

  OCR1A = 399;

}

inline void ZPC_ClockStop()
{
  TCCR1B &= ~(1 << CS10); // CS = 000, stop
}

inline void ZPC_ClockStart()
{
  TCNT1 = 0;
  
  TCCR1B |= (1 << CS10); // CS = 001, start without a prescaler
}

// ------------------------------------------------------------------Interrupt Queue------------------------------------------------------------------------------------
struct TQueue{
  uint8_t* _data;
  size_t max_size, head, tail;
};

TQueue interrupts_q;

int TQueue_init(struct TQueue* q, size_t max_size){
  q->_data = (uint8_t*)malloc(max_size);
  q->max_size = max_size;
  q->head = 0;
  q->tail = 0;
  return 0;
}

int TQueue_push(struct TQueue* q, uint8_t data){
  size_t new_tail = (q->tail+1)%q->max_size;
  if(new_tail != q->head){
    q->tail = new_tail;
    q->_data[q->tail] = data;
    return 0;
  }
  return -1;  
}

uint8_t TQueue_pop(struct TQueue* q){
  size_t new_head = (q->head + 1)%q->max_size;

  if(q->head != q->tail){
    uint8_t res = q->_data[q->head];
    q->head = new_head;
    return res;
  }
  return 0xff;  //If empty
}

uint8_t TQueue_empty(struct TQueue* q){
  return (q->head == q->tail);
}

// ------------------------------------------------------------------Interrupt management--------------------------------------------------------------------------------
// inline void ZPC_


// ----------------------------------------------------------------------Displayer---------------------------------------------------------------------------------------

static PCD8544 lcd(A0, A1, A2, A4, A3);
static ZPC_Displayer displayer(lcd);


inline void ZPC_DisplayRAM(ZPC_Displayer* displayer){
  ZPC_MemReadBlock(displayer->ram_data, displayer->ram_addr, displayer->ram_size);
  displayer->refresh();
}


// ------------------------------------------------------------------Event Handlers--------------------------------------------------------------------------------------
uint8_t serial_input_buf = 0x00;

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

void ZPC_IO_HandleSerialCommand(uint8_t command){
    command &= ~(1<<7);
    switch(command){
      case 0x00:
        break;
      default:
        break;
    }
}

void ZPC_IO_HandleSerialData(uint8_t serial_data) 
{
    serial_input_buf = serial_data; //Otherwise, treat as regular serial data
                 


}


// -----------------------------------------------------------------------Main code--------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  Serial.print("init\n");

  TQueue_init(&interrupts_q, 16);

  displayer.begin();

  ZPC_ArduinoInit();

  ZPC_MemWriteBlock(0, program, PROG_SIZE);

  // ZPC_MemReadBlock(displayer.ram_data, displayer.ram_addr, displayer.ram_size);
  // displayer.refresh();

  // displayer.set_register(rA, 0xa);
  ZPC_DisplayRAM(&displayer);

  Serial.print("Start\n");

  pinMode(USER_LED, OUTPUT);
  // pinMode(INT_, OUTPUT);    //!!
  // digitalWrite(INT_, HIGH); //!!
  ZPC_ClockConfig();
  ZPC_ClockStart(); // Mode 0 by default (Arduino clock source)

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
uint8_t mode  = 0b00;

void loop()
{

  // TODO: ugly, improve
  
  if(Serial.available()){
    uint8_t serial_input = Serial.read();
    if (serial_input&(1<<7))
    {
      ZPC_IO_HandleSerialCommand(serial_input);
    }
    else
    {
      ZPC_IO_HandleSerialData(serial_input);
    }    
  }
  // if (Serial.available())
  // {
  //   digitalWrite(INT_, LOW);
  //   int_vector = IO_INT;
  // }
  // else
  // {
  //   digitalWrite(INT_, HIGH);
  // }

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
