// #ifndef ZPC_PINOUT
// #define ZPC_PINOUT

#define D0 22 // PA0 pin 40   Z80 data bus
#define D1 23 // PA1 pin 39
#define D2 24 // PA2 pin 38
#define D3 25 // PA3 pin 37
#define D4 26 // PA4 pin 36
#define D5 27 // PA5 pin 35
#define D6 28 // PA6 pin 34
#define D7 29 // PA7 pin 33

#define AD0 30  // PC7 Z80 address bus (AD0-AD8 = A0-A8 on A04113 schematic)
#define AD1 31  // PC6
#define AD2 32  // PC5
#define AD3 33  // PC4
#define AD4 34  // PC3
#define AD5 35  // PC2
#define AD6 36  // PC1
#define AD7 37  // PC0
#define AD8 38  // PD7
#define AD9 39  // PG2
#define AD10 40 // PG1
#define AD11 41 // PG0
#define AD12 42 // PL7
#define AD13 43 // PL6
#define AD14 44 // PL5
#define AD15 45 // PL4

#define AD_PORTC_BITMASK B11111111
#define AD_PORTD_BITMASK B10000000
#define AD_PORTG_BITMASK B00000111
#define AD_PORTL_BITMASK B11110000



#define INT_ 48    // PB1 pin 2    Z80 control bus
#define BUSACK_ 51 // PB2 pin 3
#define WAIT_ 49   // PB3 pin 4

#define WR_ 53 // PB4 pin 5
#define RD_ 52 // PB5 pin 6

#define MREQ_ 46 // PB6 pin 7

#define RESET_ 5   // PB7 pin 8
#define BUSREQ_ 50 // PD6 pin 20
#define CLK 6      // PD7 pin 21

#define WAIT_RES_ 47 // PB0 pin 1    Internal control line

#define USER 13

// #endif

