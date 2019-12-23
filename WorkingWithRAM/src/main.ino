#include "Arduino.h"
#include <stdio.h>

#define D0 22 // PA0 pin 40   Z80 data bus
#define D1 23 // PA1 pin 39
#define D2 24 // PA2 pin 38
#define D3 25 // PA3 pin 37
#define D4 26 // PA4 pin 36
#define D5 27 // PA5 pin 35
#define D6 28 // PA6 pin 34
#define D7 29 // PA7 pin 33

#define AD0 30  // PC7 pin 24   Z80 address bus (AD0-AD8 = A0-A8 on A04113 schematic)
#define AD1 31  // PC6 pin 25
#define AD2 32  // PC5 pin 26
#define AD3 33  // PC4 pin 27
#define AD4 34  // PC3 pin 28
#define AD5 35  // PC2 pin 29
#define AD6 36  // PC1 pin 16
#define AD7 37  // PC0 pin 17
#define AD8 38  // PD7 pin 18
#define AD9 39  // PG2 pin 18
#define AD10 40 // PG1 pin 18
#define AD11 41 // PG0 pin 18
#define AD12 42 // PL7 pin 18
#define AD13 43 // PL6 pin 18
#define AD14 44 // PL5 pin 18
#define AD15 45 // PL4 pin 18

#define INT_ 48    // PB1 pin 2    Z80 control bus
#define BUSACK_ 51 // PB2 pin 3
#define WAIT_ 49   // PB3 pin 4

#define WR_ 53 // PB4 pin 5
#define RD_ 52 // PB5 pin 6

#define MREQ_ 46 // PB6 pin 7

#define RESET_ 5   // PB7 pin 8
#define BUSREQ_ 50 // PD6 pin 20
#define CLK 6      // PD7 pin 21

#define WAIT_RES_ 47 // PB0 pin 1    Internal control line

#define USER 13

uint8_t program = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

void
setup()
{

  char s[30];
  Serial.begin(9600);
  Serial.print("start\n");

  pinMode(WR_, OUTPUT);
  pinMode(MREQ_, OUTPUT);
  pinMode(RD_, OUTPUT);

  digitalWrite(WR_, HIGH);
  digitalWrite(MREQ_, HIGH);
  digitalWrite(RD_, HIGH);

  uint16_t address = 0xff0a;
  uint8_t data = 0x12;
  sprintf(s, "Address is %x\n", address);
  Serial.print(s);
  sprintf(s, "Data is %x\n", data);
  Serial.print(s);

  DDRA = 0xFF;  // Configure Z80 data bus D0-D7 (PA0-PA7) as output
  PORTA = data; // Current output on data bus

  for (int i = AD0; i <= AD15; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, (address >> (i - AD0)) & 1);
  }

  digitalWrite(MREQ_, LOW); // Enable external RAM /CE1 pin
  digitalWrite(WR_, LOW);   // Write data into current address of the external RAM (10us 1-0-1 pulse)
  digitalWrite(WR_, HIGH);
  digitalWrite(MREQ_, HIGH);
  PORTA = 0;

  DDRA = 0x00;
  PORTA = 0xff;
  digitalWrite(MREQ_, LOW);
  digitalWrite(RD_, LOW);

  uint8_t read_data = PINA;
  digitalWrite(RD_, HIGH);
  digitalWrite(MREQ_, HIGH);

  sprintf(s, "Data %x", read_data); //Should be equal to data
  Serial.print(s);
}

void loop()
{
  // put your main code here, to run repeatedly:
}