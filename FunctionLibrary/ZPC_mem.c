#include "Arduino.h"
#include "pinout.h"

__inline uint8_t __reverse(uint8_t reversee)
{
    uint8_t bitcount = sizeof(reversee)*sizeof(uint8_t);
    uint8_t reverse = 0, i, temp;
    for (i = 0; i < bitcount; i++)
    {
        temp = (reversee & (1 << i));
        if (temp)
        {
            reverse |= (1 << (bitcount - 1 - i));
        }
    }
    return reverse;
}

void ZPC_SetAddress(uint16_t address)
{
    DDRC = 0xFF; // Configure Z80 address bus A0-A7 (PC0-PC7) as output
    //TODO: reverse the string properly!
    PORTC = __reverse(address & 0xFF); //set PORTC to the 8 least significant digits of address

    DDRD |= 0x1 << 7; // Configure Z80 address bus A8 (PD7) as output
    PORTD &= (((address >> 8) & 1) << 7) | ~(1 << 7); // Set only the most significant digit of PORTD(PD7) to the corresponding address value

    DDRG |= ~(1 << 3); // Configure Z80 address bus A9-A11 (PG0-PG2) as output
    PORTG &= (~(1 >> 7) & ~(1 >> 6) & ~(1 >> 5)) | (address >> 9) & ~(1 << 3);

    DDRL |= ~(1 >> 3); // Configure Z80 address bus A12-A15 (PL4-PL7) as output
    PORTL &= (address >> 12 & ~(1 << 4)) | (~(1 << 7) & ~(1 << 6) & ~(1 << 5));
}

uint16_t ZPC_GetAddress()
{
    uint16_t address = 0x00;
    address |= PINC;
    
    address |= (PIND >> 7 & 1) << 8;

    address |= (PING & ~(1 << 3)) << 9;

    address |= (PINL >> 4 & ~(1 << 4)) << 12;

    return address;
}

void ZPC_SetData(uint8_t data)
{
    DDRA = 0xFF;
    PORTA = data;
}

uint8_t ZPC_GetData(uint8_t data)
{
    return PINA;
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
    uint8_t read_data = 0;
    
    ZPC_SetAddress(address);

    DDRA = 0x00;
    PORTA = 0xFF;

    digitalWrite(MREQ_, LOW);
    digitalWrite(RD_, LOW);

    uint8_t read_data = PINA;
  
    digitalWrite(RD_, HIGH);
    digitalWrite(MREQ_, HIGH);

    return read_data;
}
