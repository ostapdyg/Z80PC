#ifndef ZPC_PINOUT
#define ZPC_PINOUT

//Z80 data bus
#define D0 22 // PA0 pin 40
#define D1 23 // PA1 pin 39
#define D2 24 // PA2 pin 38
#define D3 25 // PA3 pin 37
#define D4 26 // PA4 pin 36
#define D5 27 // PA5 pin 35
#define D6 28 // PA6 pin 34
#define D7 29 // PA7 pin 33

//Z80 address bus
#define AD0 30  // PC7 
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



#define INT_ 48    
#define BUSACK_ 51 
#define WAIT_ 49

#define WR_ 53
#define RD_ 52

#define MREQ_ 46

#define RESET_ 5 
#define BUSREQ_ 50

#define CLK 11 

#define WAIT_RES_ 47 

#define USER 13

#define M1_ 15

#endif

