#include "Arduino.h"
#include "pinout.h"

void ZPC_ArduinoInit()
{
    //TODO: Implement this function
    pinMode(WR_, OUTPUT);
    pinMode(MREQ_, OUTPUT);
    pinMode(RD_, OUTPUT);

    digitalWrite(WR_, HIGH);
    digitalWrite(MREQ_, HIGH);
    digitalWrite(RD_, HIGH);
}