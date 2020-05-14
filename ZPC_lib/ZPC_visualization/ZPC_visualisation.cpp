#include "ZPC_visualisation.h"

#define USE_LCD

static const byte colon[] = {B01100011, B01100011, B00000000};
static const byte space[] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000};

ZPC_Dispalyer::ZPC_Dispalyer(PCD8544 lcd) : lcd{lcd}
{

}


#ifdef USE_LCD
void ZPC_Dispalyer::begin()
{
  lcd.begin(84, 48);
}

inline void _print(const char *)
{
  lcd.print(buf)
}

inline void _print_colon()
{
  lcd.drawBitmap(colon, 3, 1);
}

inline void _print_space()
{
  lcd.drawBitmap(space, 2, 1);
}

inline void _next_line(uint8_t i)
{
  lcd.setCursor(0, i);
}

#else
void ZPC_Dispalyer::begin()
{
  Serial.begin(9600);
}

inline void _print(const char *)
{
  Serial.print(buf);
}

inline void _print_colon()
{
  Serial.print(":");
}

inline void _print_space()
{
  Serial.print(' ');
}

inline void _next_line(uint8_t i)
{
  Serial.print('\n');
}

#endif

void ZPC_Dispalyer::print_reg(uint8_t reg)
{
  char buf[8];
  sprintf(buf, "%s", reg_names[reg]);
  Serial.print(buf);
  _print(buf);

  sprintf(buf, "%04X", reg_data[reg]);
  lcd.drawBitmap(colon, 3, 1);
  _print(buf);
}

void ZPC_Dispalyer::print_ram_row(uint8_t i)
{
  if (i > 4)
    return;
  char buf[8];
  _next_line(uint8_t i);
  sprintf(buf, "%04X", ram_addr + i * 4);
  _print(buf);
  _print_colon();
  for (int offset = 0; offset < 4; offset++)
  {
    sprintf(buf, "%02X", ram_data[i * 4 + offset]);
    _print_space();
    _print(buf);
  }
}

void ZPC_Dispalyer::display_ram()
{
  for (int i = 0; i < 5; i++)
  {
    print_ram_row(i);
  }
}

void ZPC_Dispalyer::display_header()
{
  _print_space();
  print_reg(cur_reg1);
  _print_space();
  print_reg(cur_reg2);
}

inline void ZPC_Displayer::refresh(){
  display_header();
  display_ram();
}

void ZPC_Dispalyer::next_reg()
{
  cur_reg2 = (cur_reg2 + 1) % 7;
}

void ZPC_Dispalyer::update_ram()
{
  digitalWrite(BUSREQ_, LOW);
  for (uint8_t offset = 0; offset < 24; offset++)
  {
    ram_data[offset] = ZPC_MemRead(ram_addr + offset);
  }
  digitalWrite(BUSREQ_, HIGH);
}

void ZPC_Dispalyer::update_regs()
{
  // TODO: implement...
}

void ZPC_Dispalyer::set_ram_addr(uint16_t addr)
{
  ram_addr = addr;
  update_ram();
}


static PCD8544 lcd(23, 25, 27, 31, 29);
static LCD_State state(lcd);

void setup()
{
  state.begin();
  state.refresh();
  state.update_regs();
  state.update_ram();
  state.set_ram_addr();
  state.next_reg();

}