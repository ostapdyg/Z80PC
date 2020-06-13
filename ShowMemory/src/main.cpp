#include <PCD8544.h>
#include "ZPC_funcs.h"
#include "ZPC_visualisation.h"

static PCD8544 lcd(23, 25, 27, 31, 29);
static ZPC_Displayer disp(lcd);

void setup()
{
    disp.begin();
    Serial.begin(9600);
    disp.refresh();
}

void loop()
{
}