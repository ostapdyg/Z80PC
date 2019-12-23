#include "Arduino.h"
#include "pinout.h"

void ZPC_ArduinoInit()
{
    //TODO: Implement this function
    pinMode(WR_, OUTPUT);
    pinMode(MREQ_, OUTPUT);
    pinMode(RD_, OUTPUT);
    pinMode(RESET_, OUTPUT);
    pinMode(WAIT_RES_, OUTPUT);

    digitalWrite(WR_, HIGH);
    digitalWrite(MREQ_, HIGH);
    digitalWrite(RD_, HIGH);
    digitalWrite(RESET_, LOW);
    digitalWrite(WAIT_RES_, LOW);

    pinMode(INT_, INPUT_PULLUP);
    pinMode(INT_, OUTPUT);
    digitalWrite(INT_, HIGH);

    pinMode(BUSACK_, INPUT);
    pinMode(WAIT_, INPUT);
    pinMode(BUSREQ_, INPUT_PULLUP);
    pinMode(BUSREQ_, OUTPUT);
    digitalWrite(BUSREQ_, HIGH);
}
