// // #include <PCD8544.h>
#include "ZPC_funcs.h"
// #include "ZPC_visualisation.h"

// // static PCD8544 lcd(23, 25, 27, 31, 29);
// static PCD8544 lcd(A0, A1, A2, A4, A3);
// static ZPC_Displayer disp(lcd);

// void setup()
// {
//     disp.begin();
//     Serial.begin(9600);
//     disp.refresh();
// }

// void loop()
// {
// }
enum clock_mode_en
{
    CLK_TIMER = 0,
    CLK_MAINLOOP = 1,
    CLK_BUTTON = 2,
    CLK_NONE = 3
};
enum clock_mode_en clock_mode;

void _waitSignal(uint8_t pin, int state)
{
    for (;;)
    {
        // sprintf("E")
        // Serial.print()
        while (digitalRead(pin) != state)
        {
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

    clock_mode = CLK_TIMER;
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
    Serial.print("\nWait for EXT_CLK LOW...\n");
    _waitSignal(EXT_CLOCK, LOW);
    digitalWrite(CLK, LOW);
    Serial.print("Wait for EXT_CLK HIGH...\n");
    _waitSignal(EXT_CLOCK, HIGH);
    digitalWrite(CLK, HIGH);
    break;
  case CLK_NONE:
    break;
  }
}

void setup()
{
    // disp.begin();
    // Serial.begin(9600);
    // disp.refresh();
    pinMode(CLK, OUTPUT);
    pinMode(EXT_CLOCK, INPUT_PULLUP);
    char buf[30];
    // sprintf(buf, "TCCR1A: %x\n TCCR1b: %x\n", TCCR1A, TCCR1B, OCR1A);
    // ZPC_Clock_Config();
    // delay(5000);
    // ZPC_Clock_Start();
    // delay(5000);
    // ZPC_Clock_Stop();
    clock_mode = CLK_BUTTON;

}

void loop()
{
    // digitalWrite(CLK, HIGH);
    // // delay(1000);
    // digitalWrite(CLK, LOW);
    // // delay(1000);
    ZPC_Clock_Handle();
}