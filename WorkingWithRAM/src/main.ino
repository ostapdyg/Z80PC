#include "Arduino.h"
#include <stdio.h>

#include "ZPC_funcs.h"
#include "ZPC_pinout.h"


// #define D0 22 // PA0 pin 40   Z80 data bus
// #define D1 23 // PA1 pin 39
// #define D2 24 // PA2 pin 38
// #define D3 25 // PA3 pin 37
// #define D4 26 // PA4 pin 36
// #define D5 27 // PA5 pin 35
// #define D6 28 // PA6 pin 34
// #define D7 29 // PA7 pin 33

// #define AD0 30  // PC7 pin 24   Z80 address bus (AD0-AD8 = A0-A8 on A04113 schematic)
// #define AD1 31  // PC6 pin 25
// #define AD2 32  // PC5 pin 26
// #define AD3 33  // PC4 pin 27
// #define AD4 34  // PC3 pin 28
// #define AD5 35  // PC2 pin 29
// #define AD6 36  // PC1 pin 16
// #define AD7 37  // PC0 pin 17
// #define AD8 38  // PD7 pin 18
// #define AD9 39  // PG2 pin 18
// #define AD10 40 // PG1 pin 18
// #define AD11 41 // PG0 pin 18
// #define AD12 42 // PL7 pin 18
// #define AD13 43 // PL6 pin 18
// #define AD14 44 // PL5 pin 18
// #define AD15 45 // PL4 pin 18

// #define INT_ 48    // PB1 pin 2    Z80 control bus
// #define BUSACK_ 51 // PB2 pin 3
// #define WAIT_ 49   // PB3 pin 4

// #define WR_ 53 // PB4 pin 5
// #define RD_ 52 // PB5 pin 6

// #define MREQ_ 46 // PB6 pin 7

// #define RESET_ 5   // PB7 pin 8
// #define BUSREQ_ 50 // PD6 pin 20
// #define CLK 6      // 

// #define WAIT_RES_ 47 // PB0 pin 1    Internal control line

// #define USER 13

uint8_t program[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup()
{

  char s[30];
  Serial.begin(9600);
  Serial.print("start\n");

  ZPC_ArduinoInit();

  uint16_t address = 0x00;
  uint8_t data = 0x11;
  for(uint16_t i=0; i< 80; i++){
    ZPC_MemWrite(address+i, program[i]);
  }

  for(uint16_t i=0; i< 80; i++){
    uint8_t read_data = ZPC_MemRead(address+i);
    //sprintf(s, "%x ", read_data); //Should be equal to data
    //Serial.print(s);
  }

  //ZPC_AddressSetInputPullup();
  ZPC_DataSetInputPullup();

  digitalWrite(RESET_, LOW);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW);
  digitalWrite(RESET_, HIGH);
}

void loop() 
{
  char s[30];
  digitalWrite(CLK, HIGH);
  sprintf(s, "Address : %x ", ZPC_GetAddress());
  delay(1);
  digitalWrite(CLK, LOW);
  Serial.print(s);
  //sprintf(s, "Data: %x \n", ZPC_GetData());
  delay(1000);
  
}