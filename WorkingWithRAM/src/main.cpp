#include "Arduino.h"
#include <stdio.h>

#include "ZPC_funcs.h"

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
  // uint8_t data = 0x11;
  for(uint16_t i=0; i< 80; i++){
    ZPC_MemWrite(address+i, program[i]);
  }

  for(uint16_t i=0; i< 80; i++){
    uint8_t read_data = ZPC_MemRead(address+i);
    sprintf(s, "%x ", read_data); //Should be equal to data
    Serial.print(s);
  }

  ZPC_AddressSetInputPullup();
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
  sprintf(s, "Data: %x \n", ZPC_GetData());
  delay(1000);
  
}