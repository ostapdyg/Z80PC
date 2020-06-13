#include <PCD8544.h>
#include "ZPC_funcs.h"

int randint(int n) { return rand() % (n); }
static const byte colon[] = {B01100011, B01100011, B00000000};
static const byte space[] = {B00000000, B00000000, B00000000};

struct LCD_State
{
  uint16_t ram_addr = 0x0000;
  uint8_t ram_data[24] = {0x01, 0x02, 0x03, 0x40,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00};
  uint16_t reg_data[8] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
  const char *reg_names[8] = {"AF", "BC", "DE", "HL", "IX", "IY", "SP", "PC"};
  uint8_t cur_reg1 = 7;
  uint8_t cur_reg2 = 0;
  PCD8544 lcd;

  // static const uint8_t colon[4]{ B00010000, B00110100, B00110000, B00110100, B00010000 };

  LCD_State(PCD8544 lcd) : lcd{lcd}
  {
  }

  void print_reg(uint8_t reg)
  {
    char buf[8];
    sprintf(buf, "%s", reg_names[reg]);
    Serial.print(buf);
    lcd.print(buf);

    sprintf(buf, "%04X", reg_data[reg]);
    lcd.drawBitmap(colon, 3, 1);
    lcd.print(buf);
  }

  void print_ram_row(uint8_t i)
  {
    if (i > 4)
      return;
    char buf[8];
    lcd.setCursor(0, i + 1);
    sprintf(buf, "%04X", ram_addr + i * 4);
    lcd.print(buf);
    // cld.write(" ");
    lcd.drawBitmap(colon, 3, 1);
    for (int offset = 0; offset < 4; offset++)
    {
      sprintf(buf, "%02X", ram_data[i * 4 + offset]);
      lcd.drawBitmap(space, 2, 1);
      lcd.print(buf);
    }
  }

  void display_ram()
  {
    for (int i = 0; i < 5; i++)
    {
      print_ram_row(i);
    }
  }

  void display_header()
  {
    lcd.drawBitmap(space, 2, 1);
    print_reg(cur_reg1);
    lcd.drawBitmap(space, 2, 1);
    print_reg(cur_reg2);
  }

  void begin()
  {
    lcd.begin(84, 48);
  }

  void next_reg()
  {
    cur_reg2 = (cur_reg2 + 1) % 7;
  }

  void update_ram()
  {
    digitalWrite(BUSREQ_, LOW);
    for (uint8_t offset = 0; offset < 24; offset++)
    {
      ram_data[offset] = ZPC_MemRead(ram_addr + offset);
    }
    digitalWrite(BUSREQ_, HIGH);
  }

  void update_regs()
  {
    // TODO: implement...
  }

  void set_ram_addr(uint16_t addr)
  {
    ram_addr = addr;
    update_ram();
  }
};

static PCD8544 lcd(23, 25, 27, 31, 29);
static LCD_State state(lcd);

void setup()
{
  state.begin();
  Serial.begin(9600);

  state.display_header();
  state.display_ram();
}

void loop()
{
}