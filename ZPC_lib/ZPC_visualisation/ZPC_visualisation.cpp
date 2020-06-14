#include "ZPC_visualisation.h"

// Now in header
// #ifdef USE_LCD

// ZPC_Displayer::ZPC_Displayer(PCD8544 lcd):lcd{lcd}
// {

// }

// void ZPC_Displayer::begin()
// {
//   lcd.begin(84, 48);
// }

// inline void displayer_print(const char* str)
// {
//   lcd.print(str);
// }

// inline void _print_colon()
// {
//   lcd.drawBitmap(colon, 3, 1);
// }

// inline void _print_space()
// {
//   lcd.drawBitmap(space, 2, 1);
// }

// inline void _next_line(uint8_t i)
// {
//   lcd.setCursor(0, i);
// }

// #else
// void ZPC_Dispalyer::begin()
// {
//   Serial.begin(9600);
// }

// inline void _print(const char *)
// {
//   Serial.print(buf);
// }

// inline void _print_colon()
// {
//   Serial.print(":");
// }

// inline void _print_space()
// {
//   Serial.print(' ');
// }

// inline void _next_line(uint8_t i)
// {
//   Serial.print('\n');
// }

// #endif
void ZPC_Displayer::print_reg(uint8_t reg)
{
  char buf[8];
  sprintf(buf, "%s", reg_names[reg/2]);
  // Serial.print(buf);
  _print(buf);

  sprintf(buf, "%02X%02X", reg_data[reg], reg_data[reg+1]);
  // Serial.print(buf);

  // lcd.drawBitmap(colon, 3, 1);
  _print_colon();
  _print(buf);
}

void ZPC_Displayer::print_ram_row(uint8_t i)
{
  if (i > 4)
    return;
  char buf[8];
  _next_line(i + 1);
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

void ZPC_Displayer::display_ram()
{
  for (int i = 0; i < 5; i++)
  {
    print_ram_row(i);
  }
}

void ZPC_Displayer::display_header()
{
  _next_line(0);
  _print_space();
  print_reg(cur_reg1);
  _print_space();
  print_reg(cur_reg2);
}

// In header
// inline void ZPC_Displayer::refresh(){
//   display_header();
//   display_ram();
// }

// void ZPC_Displayer::next_reg()
// {
//   cur_reg2 = (cur_reg2 + 1) % 7;
// }

void ZPC_Displayer::update_ram(uint8_t *data)
{
  // digitalWrite(BUSREQ_, LOW);
  // // for (uint8_t offset = 0; offset < 24; offset++)
  // // {
  // //   ram_data[offset] = ZPC_MemRead(ram_addr + offset);
  // // }
  // digitalWrite(BUSREQ_, HIGH);
  memcpy(ram_data, data, ram_size*sizeof(uint8_t));
}

void ZPC_Displayer::set_register(uint8_t reg, uint8_t data){
    *((uint8_t*)(reg_data) + reg) = data;
} 

void ZPC_Displayer::update_regs(uint8_t *regs)
{
  memcpy(reg_data, regs, reg_num*reg_size);
}

void ZPC_Displayer::set_ram_addr(uint16_t addr)
{
  // ram_addr = addr;
  // update_ram();
}


// static PCD8544 lcd(23, 25, 27, 31, 29);
// static LCD_State state(lcd);

// void setup()
// {
//   state.begin();
//   state.refresh();
//   state.update_regs();
//   state.update_ram();
//   state.set_ram_addr();
//   state.next_reg();

// }