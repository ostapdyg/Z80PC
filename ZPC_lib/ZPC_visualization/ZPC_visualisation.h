#include <PCD8544.h>
#include "ZPC_funcs.h"


struct ZPC_Dispalyer
{
  uint16_t ram_addr = 0x0000;
  uint8_t ram_data[24] = {0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00};
  uint16_t reg_data[8] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
  const char *reg_names[8] = {"AF", "BC", "DE", "HL", "IX", "IY", "SP", "PC"};
  uint8_t cur_reg1 = 7;
  uint8_t cur_reg2 = 0;
  PCD8544 lcd;

  ZPC_Dispalyer(PCD8544 lcd) : lcd{lcd};

  void print_reg(uint8_t reg);

  void print_ram_row(uint8_t i);


  void display_ram();


  void display_header();

  void begin();

  void next_reg();

  void update_ram();

  void update_regs();
    // TODO: implement...

  void set_ram_addr(uint16_t addr);


};
