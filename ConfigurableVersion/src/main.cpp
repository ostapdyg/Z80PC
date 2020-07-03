#include "Arduino.h"

#include <stdio.h>

#include "ZPC_funcs.h"
#include "ZPC_IO_funcs.h"
#include "ZPC_visualisation.h"
#include "../program.h"

#define program program_CUSTOM

// #define DEBUG_MODE (clock_mode==CLK_MAINLOOP)
#define DEBUG_MODE (1)
#define IO_INT 0x04

char s[30];

uint8_t W = 0;
uint8_t R = 0;
uint8_t IO = 0;
uint8_t M1 = 0;

uint16_t address = 0xffff;
uint8_t data = 0xff;
uint8_t int_vector = 0xff;

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
enum clock_mode_en
{
  CLK_TIMER = 0,
  CLK_MAINLOOP = 1,
  CLK_BUTTON = 2,
  CLK_NONE = 3
};
enum clock_mode_en clock_mode;

uint8_t wait_continue = 0;
void _waitSignal(uint8_t pin, int state)
{
    // Serial.print(".\n");
    // while(!Serial.available()){}
    // Serial.read();
    // return;

    for (;;)
    {
        // sprintf(s, "EXT_CLK: %d", digitalRead(pin));
        // Serial.print(s);
        // Serial.print()
        while (digitalRead(pin) != state)
        {
          if(Serial.available()){
            Serial.read();
            return;
          }
        }
        delay(10);
        if (digitalRead(pin) == state)
        {
            return;
        }
    }
}

void ZPC_Clock_Config()
{
    pinMode(CLK, OUTPUT);

    TCCR1B = (1 << WGM12); //Set timer mode to CTC but do not start the timer.
    TCCR1A = 0;

    TIMSK1 = 0;

    OCR1A = 399;          //Set Compare registre to 399

    // clock_mode = CLK_MAINLOOP;
}
inline void ZPC_Clock_Stop()
{
    TCCR1A &= ~(1 << COM1A0);   //Set CLK pin to free
    TCCR1B &= ~(1 << CS10); // CS = 000, stop
}

inline void ZPC_Clock_Start()
{
    TCNT1 = 0;                  //Set timer counter to zero
    TCCR1A |= (1 << COM1A0); //Set CLK pin to toggle on Compare Capture
    TCCR1B |= (1 << CS10); // CS = 001, start without a prescaler
}


void ZPC_Clock_Handle()
{
  switch (clock_mode)
  {
  case CLK_TIMER:
    break;
  case CLK_MAINLOOP:
    // delay(1);
    digitalWrite(CLK, LOW);
    digitalWrite(CLK, HIGH);
    break;
  case CLK_BUTTON:
    // Serial.print("\nWait for EXT_CLK LOW...\n");
    _waitSignal(EXT_CLOCK_, LOW);
    Serial.print(".\n");
    digitalWrite(CLK, LOW);
    // Serial.print("Wait for EXT_CLK HIGH...\n");
    _waitSignal(EXT_CLOCK_, HIGH);
    Serial.print("|\n");
    digitalWrite(CLK, HIGH);
    break;
  case CLK_NONE:
    break;
  }
}

// ------------------------------------------------------------------Clock change------------------------------------------------------------------------------------


void ZPC_Clock_Change(enum clock_mode_en new_mode)
{
  if (clock_mode == CLK_TIMER)
  {
    ZPC_Clock_Stop();
  }
  if (new_mode == CLK_TIMER)
  {
    ZPC_Clock_Start();
  }
  clock_mode = new_mode;
  return;
}

// ------------------------------------------------------------------Interrupt Queue------------------------------------------------------------------------------------
struct TQueue
{
  uint8_t *_data;
  size_t max_size, head, tail;
};

TQueue interrupts_q;
TQueue serial_data_q;

int TQueue_init(struct TQueue *q, size_t max_size)
{
  q->_data = (uint8_t *)malloc(max_size);
  q->max_size = max_size;
  q->head = 0;
  q->tail = 0;
  return 0;
}

int TQueue_push(struct TQueue *q, uint8_t data)
{
  size_t new_tail = (q->tail + 1) % q->max_size;
  if (new_tail != q->head)
  {
    q->_data[q->tail] = data;
    q->tail = new_tail;
    return 0;
  }
  return -1;
}

uint8_t TQueue_pop(struct TQueue *q)
{
  size_t new_head = (q->head + 1) % q->max_size;

  if (q->head != q->tail)
  {
    uint8_t res = q->_data[q->head];
    q->head = new_head;
    return res;
  }
  // Serial.print("-------------------------Empty!!!11\n"); // Due to some dark magick three exclamation points won't allow code to upload??
  // sprintf(s, "!!!");
  // volatile const char* s2 = "    !!!   ";

  return 0xff; //If empty
}

uint8_t TQueue_empty(struct TQueue *q)
{
  return (q->head == q->tail);
}

// ------------------------------------------------------------------Interrupt management--------------------------------------------------------------------------------
// inline void ZPC_

// ----------------------------------------------------------------------Displayer---------------------------------------------------------------------------------------

static PCD8544 lcd(A0, A1, A2, A4, A3);
static ZPC_Displayer displayer(lcd);

inline void ZPC_DisplayRAM(ZPC_Displayer *displayer)
{
  ZPC_MemReadBlock(displayer->ram_data, displayer->ram_addr, displayer->ram_size);
  displayer->refresh();
}

// ------------------------------------------------------------------Event Handlers--------------------------------------------------------------------------------------
// uint8_t serial_input_buf = 0x00;
// uint8_t buf_has_data = 0;

void ZPC_IO_HandleWrite(uint16_t address, uint8_t data)
{
  uint8_t port = address & 0xff;
  // ZPC_IO_Serial_WriteByte(data);
  switch (port)
  {
  case 0x00: // Wait till a key is pressed
    sprintf(s, "Press any key to continue:\n");
    Serial.print(s);
    while (!Serial.available())
      ;
    Serial.read();
    break;
  case 0x0a: // Save byte to ROM
    ZPC_IO_ArduinoROM_WriteByte(address, data);
    break;
  case 0x01: // Print byte as char
    ZPC_IO_Serial_WriteByte(data);
    break;
  case 0x02: // Print byte as hex
    sprintf(s, "%02X", data);
    Serial.print(s);
    break;
  case 0x0f:
    static uint32_t running_time;
    sprintf(s, "Running time: %li ms \n", millis() - running_time);
    Serial.print(s);
    running_time = millis();
    break;
  case 0x30: //Debug output: A
  case 0x31: //Debug output: F
  case 0x32: //Debug output: B
  case 0x33: //Debug output: C
  case 0x34: //Debug output: D
  case 0x35: //Debug output: E
  case 0x36: //Debug output: H
  case 0x37: //Debug output: L
  case 0x38: //Debug output: IXh
  case 0x39: //Debug output: IXl
  case 0x3a: //Debug output: IYh
  case 0x3b: //Debug output: IYl
  case 0x3c: //Debug output: SPh
  case 0x3d: //Debug output: SPl
  case 0x3e: //Debug output: PCh
  case 0x3f: //Debug output: PCl
    // sprintf(s, "Debug: %X - %02X\n", port&0xf, data);
    // Serial.print(s);
    displayer.set_register(port - 0x30, data);
    break;
  case 0x40: //Debug output: refresh displayer
    displayer.refresh();
    break;
  default:
    ZPC_IO_Serial_WriteByte(data);
  }
};

//Returns data to feed to Z80
uint8_t ZPC_IO_HandleRead(uint16_t address)
{
  uint8_t data_in = 0xff;
  uint8_t port = address & 0xff;
  switch (port)
  {
  case 0x0a:
    data_in = ZPC_IO_ArduinoROM_ReadByte(address);
    break;
  case 0x01:
    // data_in = ZPC_IO_Serial_ReadByte(); //Todo: pop 1 byte from serial queue
    data_in = TQueue_pop(&serial_data_q);
    break;
  default:
    // data_in = ZPC_IO_Serial_ReadByte(); //Todo: pop 1 byte from serial queue
    data_in = TQueue_pop(&serial_data_q);
  }
  return data_in;
}

void ZPC_Serial_HandleCommand(uint8_t command)
{
  sprintf(s, "----Command from Serial: %02X\n", command);
  command |= (1 << 7);
  switch (command)
  {
  case 0x00:
    break; //DOTO
  case 0xB9:
    ZPC_Clock_Change(CLK_MAINLOOP);
    break;
  case 0x86:
    ZPC_Clock_Change(CLK_TIMER);
    break;
  case 0x83:
    ZPC_Clock_Change(CLK_BUTTON);
    break;
  default:
    break;
  }
  // ZPC_Clock_Change(CLK_MAINLOOP);
}

void ZPC_Serial_HandleData(uint8_t serial_data)
{
  sprintf(s, "----Data from Serial: %02X\n", serial_data);
  TQueue_push(&serial_data_q, serial_data);
  TQueue_push(&interrupts_q, IO_INT);
}

void ZPC_Serial_Handle()
{
  // ZPC_Clock_Change(CLK_MAINLOOP);
  if (Serial.available())
  {
    uint8_t serial_input = Serial.read();
    if (serial_input & (1 << 7)) // Last bit set -> it is a command
    {
      ZPC_Serial_HandleCommand(serial_input);
    }
    else //Otherwise, treat as regular serial data
    {
      ZPC_Serial_HandleData(serial_input);
    }
  }
}

uint8_t interrupt_in_progress = 0;
uint8_t interrupt_vector = 0x00;

void ZPC_Interrupts_HandleSend()
{
  if (!interrupt_in_progress)
  {
    if (!TQueue_empty(&interrupts_q))
    {
      interrupt_vector = TQueue_pop(&interrupts_q);
      digitalWrite(INT_, LOW);
      interrupt_in_progress = 1;
    }
    // else{
    //   digitalWrite(INT_, HIGH);
    // }
  }
}

uint8_t data_is_output = 0;
uint8_t data_is_set = 0;
uint8_t io_delay = 0;
// uint8_t data = 0x00;
void ZPC_IO_Handle()
{
  if (IO)
  {
    if(!data_is_set)
    {
      if (R)
      {
        data = ZPC_IO_HandleRead(address);
        data_is_set = 1;
        data_is_output = 1;
        sprintf(s, "  Handling read: %x\n", data);
        Serial.print(s);
        // ZPC_DataSetOutput();
        // ZPC_SetData(data);
      }
      else if (W)
      {
        ZPC_IO_HandleWrite(address, data);
      }
      else if (M1) //M1 + IO means interrupt vector request for type2 interrupt
      {

        data = interrupt_vector;
        data_is_output = 1;
        data_is_set = 1;
        sprintf(s, "  Int vector: %x\n", data);
        Serial.print(s);
        interrupt_in_progress = 0; //Interrupt will end in next cycle
        digitalWrite(INT_, HIGH);
      }
      digitalWrite(BUSREQ_, LOW);
      digitalWrite(WAIT_RES_, LOW);
      if (data_is_output)
      {
        ZPC_DataSetOutput();
        ZPC_SetData(data);
      }
    }
  }
  else
  {
    if (data_is_output)
    {
      ZPC_DataSetInputPullup();
      data_is_output = 0;
    }
    data_is_set = 0;
    digitalWrite(WAIT_RES_, HIGH);
    digitalWrite(BUSREQ_, HIGH);
  }
}

// -----------------------------------------------------------------------Main code--------------------------------------------------------------------------------------

// -----------------------------------------------------------------------Setup--------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  Serial.print("init\n");

  TQueue_init(&interrupts_q, 256);
  TQueue_init(&serial_data_q, 256);

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

  pinMode(timingPin, INPUT_PULLUP);
  pinMode(STATE_PIN0, INPUT_PULLUP);
  pinMode(STATE_PIN1, INPUT_PULLUP);
  pinMode(CNG_MODE_, INPUT_PULLUP);    

  clock_mode = CLK_TIMER;
  pinMode(EXT_CLOCK_, INPUT_PULLUP);
  ZPC_Clock_Config();
  ZPC_Clock_Start(); // Mode 0 by default (Arduino clock source)
  ZPC_Clock_Change(CLK_MAINLOOP);

  ZPC_ProcStart();

  digitalWrite(RESET_, LOW);
  delay(1);
  for(int i=0; i<10;i++){
    ZPC_Clock_Handle();
  }
  // ZPC_Clock_Change(CLK_MAINLOOP);
  digitalWrite(RESET_, HIGH);
}

int clk_s = 0;

// -----------------------------------------------------------------------Main loop--------------------------------------------------------------------------------------

void loop()
{
  delay(100);

  if !digital_read(CNG_MODE_) 
  {
    delay(50);
    int new_tick_mode = digitalRead(STATE_PIN0) | (digitalRead(STATE_PIN1)<<1);
    if (clock_mode != new_tick_mode) 
    {
      ZPC_Clock_Change(new_tick_mode);
    }
  }

  ZPC_Serial_Handle();
  ZPC_Clock_Handle();

  ZPC_Interrupts_HandleSend(); //Check interrupt queue and set INT to 0 or 1 if neded


  IO = !digitalRead(WAIT_);
  W = !digitalRead(WR_);
  R = !digitalRead(RD_);
  M1 = !digitalRead(M1_);

  address = ZPC_GetAddress();
  data = ZPC_GetData();

  ZPC_IO_Handle();


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

  

}
