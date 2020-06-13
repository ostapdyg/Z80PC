#ifndef ZPC_VISUALISATION_H
#define ZPC_VISUALISATION_H

#include <PCD8544.h>
#include "ZPC_funcs.h"

#define USE_LCD

static const byte colon[] = {B01100011, B01100011, B00000000};
static const byte space[] = {B00000000, B00000000, B00000000};

struct ZPC_Displayer
{
  static const uint8_t ram_size = 24;
  static const uint8_t reg_num = 8;
  static const uint8_t reg_size = sizeof(uint16_t);

  uint16_t ram_addr = 0x0000;
  uint8_t ram_data[ram_size] = {0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00};
  uint16_t reg_data[reg_num] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
  const char *reg_names[reg_num] = {"AF", "BC", "DE", "HL", "IX", "IY", "SP", "PC"};
  uint8_t cur_reg1 = reg_num-1;
  uint8_t cur_reg2 = 0;

  void print_reg(uint8_t reg);

  void print_ram_row(uint8_t i);

  void display_ram();

  void display_header();

  // void begin();

  void switch_regs();

  void update_ram(uint8_t *data);

  void update_regs(uint8_t *regs);

  void set_ram_addr(uint16_t addr);

  inline void refresh(){
    display_header();
    display_ram();
  }



#ifdef USE_LCD
  PCD8544 lcd;
// Not glad of it...
  inline ZPC_Displayer(PCD8544 lcd):lcd{lcd}{

  }

  void inline begin()
  {
    lcd.begin(84, 48);
  }

  inline void _print(const char *str)
  {
    lcd.print(str);
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
  inline ZPC_Displayer():{

  }

  void inline begin()
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
};

#endif