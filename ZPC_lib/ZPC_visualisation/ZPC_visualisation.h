#ifndef ZPC_VISUALISATION_H
#define ZPC_VISUALISATION_H

#include <PCD8544.h>
#include "Arduino.h"

// #include "ZPC_funcs.h"

#define USE_LCD

static const byte colon[] = {B01100011, B01100011, B00000000};
static const byte space[] = {B00000000, B00000000, B00000000};

#define rAF 0
#define rA 0    //A register
#define rF 1    //A register

#define rBC 2
#define rB 2    //A register
#define rC 3    //A register

#define rDE 4
#define rD 4    //A register
#define rE 5    //A register

#define rHL 6
#define rHLh 6  //A register
#define rHLl 7  //A register

#define rIX 8
#define rIXh 8  //A register
#define rIXl 9  //A register

#define rIY 10
#define rIYh 10 //A register
#define rIYl 11 //A register

#define rSP 12
#define rSPh 12 //A register
#define rSPl 13 //A register

#define rPC 14
#define rPCh 14 //A register
#define rPCl 15 //A register

struct ZPC_Displayer
{
  static const uint8_t ram_size = 24;
  static const uint8_t reg_num = 16;
  static const uint8_t reg_size = sizeof(uint16_t);

  uint16_t ram_addr = 0x0000;
  uint8_t ram_data[ram_size] = {0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00};
  // uint16_t reg_data[reg_num] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
  uint8_t reg_data[reg_num] = {0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00};
  const char *reg_names[reg_num/2] = {"AF", "BC", "DE", "HL", "IX", "IY", "SP", "PC"};
  uint8_t cur_reg1 = rSP;
  uint8_t cur_reg2 = rAF;

  void print_reg(uint8_t reg);

  void print_ram_row(uint8_t i);

  void display_ram();

  void display_header();

  // void begin();

  void switch_regs();

  void update_ram(uint8_t *data);

  void update_regs(uint8_t *regs);

  void set_register(uint8_t reg, uint8_t data);

  void set_ram_addr(uint16_t addr);

  inline void refresh()
  {
    display_header();
    display_ram();
  }

#ifdef USE_LCD
  PCD8544 lcd;
  // Not glad of it...
  inline ZPC_Displayer(PCD8544 lcd) : lcd{lcd}
  {
  }

  void inline begin()
  {
    lcd.begin(84, 48);
  }

  inline void _print(const char *str)
  {
    lcd.print(str);
    Serial.print(str);

  }

  inline void _print_colon()
  {
    lcd.drawBitmap(colon, 3, 1);
        Serial.print(":");

  }

  inline void _print_space()
  {
    lcd.drawBitmap(space, 2, 1);
        Serial.print(' ');

  }

  inline void _next_line(uint8_t i)
  {
    lcd.setCursor(0, i);
        Serial.print('\n');

  }

#else
  inline ZPC_Displayer()
  {
  }

  void inline begin()
  {
    Serial.begin(9600);
  }

  inline void _print(const char *str)
  {
    Serial.print(str);
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