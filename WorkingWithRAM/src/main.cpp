#include "Arduino.h"
#include <stdio.h>

#include "ZPC_funcs.h"

uint8_t program[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
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
  for(uint16_t i=0; i< 40; i++){
    ZPC_MemWrite(address+i, program[i]);
  }

  for(uint16_t i=0; i< 40; i++){
    uint8_t read_data = ZPC_MemRead(address+i);
    sprintf(s, "%x ", read_data); //Should be equal to data
    Serial.print(s);
  }
  ZPC_AddressSetInputPullup();
  ZPC_DataSetInputPullup();
  pinMode(MREQ_, INPUT);                   // Configure MREQ_ as input with pull-up
  pinMode(RD_, INPUT);                     // Configure RD_ as input with pull-up
  pinMode(WR_, INPUT);

  //Set the RS trigger
  digitalWrite(WAIT_RES_, LOW);
  digitalWrite(WAIT_RES_, HIGH);


  //Reset has to be low for at least 4 CLK ticks
  digitalWrite(RESET_, LOW);
  for(uint8_t i=0; i<=10; i++){
    digitalWrite(CLK, HIGH);
    delay(1);
    digitalWrite(CLK, LOW);
    delay(1);
  }
  digitalWrite(RESET_, HIGH);
}

void loop() 
{
  char s[30];
  digitalWrite(CLK, LOW);
    if(!digitalRead(WR_)){
    Serial.print("Write ");
  }
  else if(!digitalRead(RD_)){
    Serial.print("Read ");
  }
  sprintf(s, "Address : %04x Data: %x \n", ZPC_GetAddress(), ZPC_GetData());
  Serial.print(s);
  digitalWrite(CLK, HIGH);
  delay(1000);

}