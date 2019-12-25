#include "Arduino.h"

#include "ZPC_funcs.h"

__inline uint8_t __reverse(uint8_t reversee)
{
    uint8_t bitcount = 8;
    uint8_t reverse = 0, i;
    for (i = 0; i < bitcount; i++)
    {
        reverse |= (((reversee >> i) & 1) << (bitcount - 1 - i));
    }
    return reverse;
}

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
    DDRG &= ~AD_PORTG_BITMASK; // (PG0-PG2) as output
    DDRL &= ~AD_PORTL_BITMASK; // (PL4-PL7) as output

    PORTC |= AD_PORTC_BITMASK; // A0-A7 (PC0-PC7) as pullup
    PORTD |= AD_PORTD_BITMASK; // A8 (PD7) as pullup
    PORTG |= AD_PORTG_BITMASK; // (PG0-PG2) as pullup
    PORTL |= AD_PORTL_BITMASK; // (PL4-PL7) as pullup
}

void ZPC_DataSetOutput(void)
{
    DDRA = 0xFF;
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
    uint16_t reversed_address = __reverse(address);
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
    uint16_t reversed_address = 0x00;
    //reversed_address: C7 C6 C5 C4 C3 C2 C1 C0 D7 G2 G1 G0 L7 L6 L5 L4

    reversed_address |= ((PINC & AD_PORTC_BITMASK) << 8);

    reversed_address |= ((PIND & AD_PORTD_BITMASK) << 0);

    reversed_address |= ((PING & AD_PORTG_BITMASK) << 4);

    reversed_address |= ((PINL & AD_PORTL_BITMASK) >> 4);

    return __reverse(reversed_address);
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
    pinMode(MREQ_, OUTPUT);
    pinMode(RD_, OUTPUT);
    pinMode(RESET_, OUTPUT);
    pinMode(WAIT_RES_, OUTPUT);

    digitalWrite(WR_, HIGH);
    digitalWrite(MREQ_, HIGH);
    digitalWrite(RD_, HIGH);
    digitalWrite(RESET_, LOW);
    digitalWrite(WAIT_RES_, LOW);

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

