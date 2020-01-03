#include "Arduino.h"

#include "ZPC_funcs.h"


__inline uint16_t __reverse(uint16_t reversee, uint8_t bitcount)
{
    uint8_t i;
    uint16_t reverse = 0;
    for (i = 0; i < bitcount; i++)
    {
        reverse |= (((reversee >> i) & 1) << (bitcount - 1 - i));
    }
    return reverse;
}

// __inline uint8_t __reverse8(uint8_t reversee)
// {
//     return __reverse(reversee, 8);
// }

// __inline uint16_t __reverse16(uint16_t reversee){
//     return __reverse(reversee, 16);
// }

void ZPC_AddressSetOutput(void)
{
    DDRC |= AD_PORTC_BITMASK; // Configure Z80 address bus A0-A7 (PC0-PC7) as output
    DDRD |= AD_PORTD_BITMASK; // Configure Z80 address bus A8 (PD7) as output
    DDRG |= AD_PORTG_BITMASK; // Configure Z80 address bus A9-A11 (PG0-PG2) as output
    DDRL |= AD_PORTL_BITMASK; // Configure Z80 address bus A12-A15 (PL4-PL7) as output
}

void ZPC_AddressSetInputPullup(void)
{
    DDRC &= ~AD_PORTC_BITMASK; // A0-A7 (PC0-PC7) as output
    DDRD &= ~AD_PORTD_BITMASK; // A8 (PD7) as output
    DDRG &= ~AD_PORTG_BITMASK; // A9-A11 (PG0-PG2) as output
    DDRL &= ~AD_PORTL_BITMASK; // A12-A15(PL4-PL7) as output

    PORTC |= AD_PORTC_BITMASK; // A0-A7 (PC0-PC7) as pullup
    PORTD |= AD_PORTD_BITMASK; // A8 (PD7) as pullup
    PORTG |= AD_PORTG_BITMASK; // A9-A11 (PG0-PG2) as pullup
    PORTL |= AD_PORTL_BITMASK; // A12-A15 (PL4-PL7) as pullup
}

void ZPC_DataSetOutput(void)
{
    DDRA = 0xFF;
    //PORTA = 0x00;
}

void ZPC_DataSetInputPullup(void)
{
    DDRA = 0x00;
    PORTA = 0xFF;
}

void ZPC_SetData(uint8_t data)
{
    ZPC_DataSetOutput();
    PORTA = data;
}

uint8_t ZPC_GetData()
{
    return PINA;
}


void ZPC_SetAddress(uint16_t address)
{
    uint16_t reversed_address = __reverse16(address);
    //reversed_address: C7 C6 C5 C4 C3 C2 C1 C0 D7 G2 G1 G0 L7 L6 L5 L4
    ZPC_AddressSetOutput();

    PORTC = (PORTC & ~AD_PORTC_BITMASK) |
            (AD_PORTC_BITMASK & (reversed_address >> 8));

    PORTD = (PORTD & ~AD_PORTD_BITMASK) |
            (AD_PORTD_BITMASK & (reversed_address >> 0));

    PORTG = (PORTG & ~AD_PORTG_BITMASK) |
            (AD_PORTG_BITMASK & (reversed_address >> 4));

    PORTL = (PORTL & ~AD_PORTL_BITMASK) |
            (AD_PORTL_BITMASK & (reversed_address << 4));
}

uint16_t ZPC_GetAddress()
{
    uint16_t reversed_address = 0x0000;
    //reversed_address: C7 C6 C5 C4 C3 C2 C1 C0 D7 G2 G1 G0 L7 L6 L5 L4

    reversed_address |= ((PINC & AD_PORTC_BITMASK) << 8);

    reversed_address |= ((PIND & AD_PORTD_BITMASK) << 0);

    reversed_address |= ((PING & AD_PORTG_BITMASK) << 4);

    reversed_address |= ((PINL & AD_PORTL_BITMASK) >> 4);

    return __reverse16(reversed_address);
}


void ZPC_MemWrite(uint16_t address, uint8_t data)
{
    ZPC_SetAddress(address);
    ZPC_SetData(data);

    digitalWrite(MREQ_, LOW); // Enable external RAM /CE1 pin
    digitalWrite(WR_, LOW);   // Write data into current address of the external RAM (10us 1-0-1 pulse)

    digitalWrite(WR_, HIGH);
    digitalWrite(MREQ_, HIGH);
}

uint8_t ZPC_MemRead(uint16_t address)
{
    ZPC_SetAddress(address);

    ZPC_DataSetInputPullup();

    digitalWrite(MREQ_, LOW);
    digitalWrite(RD_, LOW);

    uint8_t read_data = ZPC_GetData();

    digitalWrite(RD_, HIGH);
    digitalWrite(MREQ_, HIGH);

    return read_data;
}


void ZPC_ArduinoInit()
{
    //TODO: Implement this function
    pinMode(WR_, OUTPUT);
    digitalWrite(WR_, HIGH);

    pinMode(MREQ_, OUTPUT);
    digitalWrite(MREQ_, HIGH);
    
    pinMode(RD_, OUTPUT);
    digitalWrite(RD_, HIGH);
    
    pinMode(RESET_, OUTPUT);
    digitalWrite(RESET_, HIGH);

    pinMode(WAIT_RES_, OUTPUT);
    digitalWrite(WAIT_RES_, LOW);

    pinMode(CLK, OUTPUT);
    digitalWrite(CLK, LOW);
    ZPC_AddressSetInputPullup();
    ZPC_DataSetInputPullup();

    pinMode(INT_, INPUT_PULLUP);
    pinMode(INT_, OUTPUT);
    digitalWrite(INT_, HIGH);

    pinMode(BUSACK_, INPUT);
    pinMode(WAIT_, INPUT);
    pinMode(BUSREQ_, INPUT_PULLUP);
    pinMode(BUSREQ_, OUTPUT);
    digitalWrite(BUSREQ_, HIGH);

}

void ZPC_ProcStart(){
  ZPC_AddressSetInputPullup();
  ZPC_DataSetInputPullup();
  pinMode(MREQ_, INPUT);                   // Configure MREQ_ as input with pull-up
  pinMode(RD_, INPUT);                     // Configure RD_ as input with pull-up
  pinMode(WR_, INPUT);
  digitalWrite(WAIT_RES_, LOW);            //Set the RS trigger
  digitalWrite(WAIT_RES_, HIGH);

  pinMode(BUSREQ_, OUTPUT);
  digitalWrite(BUSREQ_, HIGH);
  


  //Reset 1-0-1 pulse for at least 4 CLK ticks
  digitalWrite(RESET_, LOW);
  for(uint8_t i=0; i<=10; i++){
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
  digitalWrite(RESET_, HIGH);

}