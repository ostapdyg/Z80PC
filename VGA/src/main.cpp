#include "Arduino.h"

#include <VGAXUA.h>
#include <SoftwareSerial.h>


//font generated from 1bitfont - by Sandro Maffiodo
//#undef ATMEGA2560_MAXRES
//--------------------------------6x6Font----------------------------------------------------
#define FNT_NANOFONT_HEIGHT 6
#define FNT_NANOFONT_MAX_WIDTH 6
#define FNT_NANOFONT_SYMBOLS_COUNT 96
//data size=570 bytes
const unsigned char fnt_nanofont_data[FNT_NANOFONT_SYMBOLS_COUNT][1+FNT_NANOFONT_HEIGHT] PROGMEM={
  { 1, 128, 128, 128, 0, 128, 0, }, //glyph '!' code=0
  { 3, 160, 160, 0, 0, 0, 0, }, //glyph '"' code=1
  { 5, 80, 248, 80, 248, 80, 0, },  //glyph '#' code=2
  { 5, 120, 160, 112, 40, 240, 0, },  //glyph '$' code=3
  { 5, 136, 16, 32, 64, 136, 0, },  //glyph '%' code=4
  { 5, 96, 144, 104, 144, 104, 0, },  //glyph '&' code=5
  { 2, 128, 64, 0, 0, 0, 0, },  //glyph ''' code=6
  { 2, 64, 128, 128, 128, 64, 0, }, //glyph '(' code=7
  { 2, 128, 64, 64, 64, 128, 0, },  //glyph ')' code=8
  { 3, 0, 160, 64, 160, 0, 0, },  //glyph '*' code=9
  { 3, 0, 64, 224, 64, 0, 0, }, //glyph '+' code=10
  { 2, 0, 0, 0, 0, 128, 64, },  //glyph ',' code=11
  { 3, 0, 0, 224, 0, 0, 0, }, //glyph '-' code=12
  { 1, 0, 0, 0, 0, 128, 0, }, //glyph '.' code=13
  { 5, 8, 16, 32, 64, 128, 0, },  //glyph '/' code=14
  { 4, 96, 144, 144, 144, 96, 0, }, //glyph '0' code=15
  { 3, 64, 192, 64, 64, 224, 0, },  //glyph '1' code=16
  { 4, 224, 16, 96, 128, 240, 0, }, //glyph '2' code=17
  { 4, 224, 16, 96, 16, 224, 0, },  //glyph '3' code=18
  { 4, 144, 144, 240, 16, 16, 0, }, //glyph '4' code=19
  { 4, 240, 128, 224, 16, 224, 0, },  //glyph '5' code=20
  { 4, 96, 128, 224, 144, 96, 0, }, //glyph '6' code=21
  { 4, 240, 16, 32, 64, 64, 0, }, //glyph '7' code=22
  { 4, 96, 144, 96, 144, 96, 0, },  //glyph '8' code=23
  { 4, 96, 144, 112, 16, 96, 0, },  //glyph '9' code=24
  { 1, 0, 128, 0, 128, 0, 0, }, //glyph ':' code=25
  { 2, 0, 128, 0, 0, 128, 64, },  //glyph ';' code=26
  { 3, 32, 64, 128, 64, 32, 0, }, //glyph '<' code=27
  { 3, 0, 224, 0, 224, 0, 0, }, //glyph '=' code=28
  { 3, 128, 64, 32, 64, 128, 0, },  //glyph '>' code=29
  { 4, 224, 16, 96, 0, 64, 0, },  //glyph '?' code=30
  { 4, 96, 144, 176, 128, 112, 0, },  //glyph '@' code=31
  { 4, 96, 144, 240, 144, 144, 0, },  //glyph 'A' code=32
  { 4, 224, 144, 224, 144, 224, 0, }, //glyph 'B' code=33
  { 4, 112, 128, 128, 128, 112, 0, }, //glyph 'C' code=34
  { 4, 224, 144, 144, 144, 224, 0, }, //glyph 'D' code=35
  { 4, 240, 128, 224, 128, 240, 0, }, //glyph 'E' code=36
  { 4, 240, 128, 224, 128, 128, 0, }, //glyph 'F' code=37
  { 4, 112, 128, 176, 144, 112, 0, }, //glyph 'G' code=38
  { 4, 144, 144, 240, 144, 144, 0, }, //glyph 'H' code=39
  { 3, 224, 64, 64, 64, 224, 0, },  //glyph 'I' code=40
  { 4, 240, 16, 16, 144, 96, 0, },  //glyph 'J' code=41
  { 4, 144, 160, 192, 160, 144, 0, }, //glyph 'K' code=42
  { 4, 128, 128, 128, 128, 240, 0, }, //glyph 'L' code=43
  { 5, 136, 216, 168, 136, 136, 0, }, //glyph 'M' code=44
  { 4, 144, 208, 176, 144, 144, 0, }, //glyph 'N' code=45
  { 4, 96, 144, 144, 144, 96, 0, }, //glyph 'O' code=46
  { 4, 224, 144, 224, 128, 128, 0, }, //glyph 'P' code=47
  { 4, 96, 144, 144, 144, 96, 16, },  //glyph 'Q' code=48
  { 4, 224, 144, 224, 160, 144, 0, }, //glyph 'R' code=49
  { 4, 112, 128, 96, 16, 224, 0, }, //glyph 'S' code=50
  { 3, 224, 64, 64, 64, 64, 0, }, //glyph 'T' code=51
  { 4, 144, 144, 144, 144, 96, 0, },  //glyph 'U' code=52
  { 3, 160, 160, 160, 160, 64, 0, },  //glyph 'V' code=53
  { 5, 136, 168, 168, 168, 80, 0, },  //glyph 'W' code=54
  { 4, 144, 144, 96, 144, 144, 0, },  //glyph 'X' code=55
  { 3, 160, 160, 64, 64, 64, 0, },  //glyph 'Y' code=56
  { 4, 240, 16, 96, 128, 240, 0, }, //glyph 'Z' code=57
  { 2, 192, 128, 128, 128, 192, 0, }, //glyph '[' code=58
  { 5, 128, 64, 32, 16, 8, 0, },  //glyph '\' code=59
  { 2, 192, 64, 64, 64, 192, 0, },  //glyph ']' code=60
  { 5, 32, 80, 136, 0, 0, 0, }, //glyph '^' code=61
  { 4, 0, 0, 0, 0, 240, 0, }, //glyph '_' code=62
  { 2, 128, 64, 0, 0, 0, 0, },  //glyph '`' code=63
  { 3, 0, 224, 32, 224, 224, 0, },  //glyph 'a' code=64
  { 3, 128, 224, 160, 160, 224, 0, }, //glyph 'b' code=65
  { 3, 0, 224, 128, 128, 224, 0, }, //glyph 'c' code=66
  { 3, 32, 224, 160, 160, 224, 0, },  //glyph 'd' code=67
  { 3, 0, 224, 224, 128, 224, 0, }, //glyph 'e' code=68
  { 2, 64, 128, 192, 128, 128, 0, },  //glyph 'f' code=69
  { 3, 0, 224, 160, 224, 32, 224, },  //glyph 'g' code=70
  { 3, 128, 224, 160, 160, 160, 0, }, //glyph 'h' code=71
  { 1, 128, 0, 128, 128, 128, 0, }, //glyph 'i' code=72
  { 2, 0, 192, 64, 64, 64, 128, },  //glyph 'j' code=73
  { 3, 128, 160, 192, 160, 160, 0, }, //glyph 'k' code=74
  { 1, 128, 128, 128, 128, 128, 0, }, //glyph 'l' code=75
  { 5, 0, 248, 168, 168, 168, 0, }, //glyph 'm' code=76
  { 3, 0, 224, 160, 160, 160, 0, }, //glyph 'n' code=77
  { 3, 0, 224, 160, 160, 224, 0, }, //glyph 'o' code=78
  { 3, 0, 224, 160, 160, 224, 128, }, //glyph 'p' code=79
  { 3, 0, 224, 160, 160, 224, 32, },  //glyph 'q' code=80
  { 3, 0, 224, 128, 128, 128, 0, }, //glyph 'r' code=81
  { 2, 0, 192, 128, 64, 192, 0, },  //glyph 's' code=82
  { 3, 64, 224, 64, 64, 64, 0, }, //glyph 't' code=83
  { 3, 0, 160, 160, 160, 224, 0, }, //glyph 'u' code=84
  { 3, 0, 160, 160, 160, 64, 0, },  //glyph 'v' code=85
  { 5, 0, 168, 168, 168, 80, 0, },  //glyph 'w' code=86
  { 3, 0, 160, 64, 160, 160, 0, },  //glyph 'x' code=87
  { 3, 0, 160, 160, 224, 32, 224, },  //glyph 'y' code=88
  { 2, 0, 192, 64, 128, 192, 0, },  //glyph 'z' code=89
  { 3, 96, 64, 192, 64, 96, 0, }, //glyph '{' code=90
  { 1, 128, 128, 128, 128, 128, 0, }, //glyph '|' code=91
  { 3, 192, 64, 96, 64, 192, 0, },  //glyph '}' code=92
  { 3, 96, 192, 0, 0, 0, 0, },  //glyph '~' code=93
  { 4, 48, 64, 224, 64, 240, 0, },  //glyph '£' code=94
  { 5, 252, 252, 252, 252, 252, 252, },  //glyph 'del' code=95
  //{ 5, 0, 168, 168, 168, 80, 0, },
};
//-------------------------------8x8Font------------------------------------------------------
// #include "font8x8.h"

// #define FNT_NANOFONT_HEIGHT 8
// #define FNT_NANOFONT_MAX_WIDTH 8
// #define FNT_NANOFONT_SYMBOLS_COUNT 128
// #define fnt_nanofont_data font8x8_basic
// //-------------------------------------------------------------------------------------------

VGAXUA vga;

static const char str0[] PROGMEM="        VGAXUA Hello World!";
static const char str1[] PROGMEM="        by Sandro Maffiodo";
static int x= 0;
static int y= 0;
static const char my_str[] PROGMEM="";
//SoftwareSerial mySerial(50, 51); //RX, TX
char strs[2];
char temp;
static char* blinker = "_";
uint16_t blink = 0;
bool toggle = true;

void setup() {
  Serial3.begin(9600);
  //mySerial.begin(9600);
  vga.begin();
  vga.clear(0);
  strs[1] = '\0';
  strs[0] = '_';
  //vga.clear(11);

 
  //vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str1, x, VGAX_HEIGHT/2+4, 11);
  
}

void draw_box(uint8_t x, uint8_t y, uint8_t color)
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 7; j++)
    {
      vga.putpixel(x+j,y+i, 0);
    }
  }
}

void loop() {
  
  // blink %= 20000;
  // if (!blink)
  // {
  //   draw_box(x,y,toggle);
  //   toggle = !toggle;
  // }
  // blink++;
  if (blink > 10)
  {
    toggle = !toggle;
    blink = 0;
  }
  vga.printSRAM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, blinker, x, y, 0);
  if (Serial3.available())
  {
    temp = Serial3.read();

    strs[0] = temp;
    switch (temp)
    {
    case 13: //Enter
      x = 0;
      y += FNT_NANOFONT_HEIGHT;
      break;

    case 0xc: // Escape
      vga.clear(0); 
      x = 0; 
      y = 0;
      break;
    
    case 127: //Delete
      
      //vga.printSRAM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, strs, x, y, 11);
      if (x == 0)
      {
        if (y != 0)
        {
          x = VGAX_WIDTH - FNT_NANOFONT_MAX_WIDTH - 2;
          y -= FNT_NANOFONT_HEIGHT;
        }
      }
      else
      {
        x -= FNT_NANOFONT_MAX_WIDTH;
      }
      
      draw_box(x,y,0);


      //vga.printSRAM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, strs, x, y, 0);
      break;
    
    default:
      vga.printSRAM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, strs, x, y, 11);
      x += FNT_NANOFONT_MAX_WIDTH;
      break;
    }
    
    if (x >= VGAX_WIDTH - FNT_NANOFONT_MAX_WIDTH) 
    {  
      x = 0;
      y += FNT_NANOFONT_HEIGHT;
    }

    if (y >= VGAX_HEIGHT) 
    {
      y = 0;
      vga.clear(0);
      // maybe x = 0;
    }
  }
  
  blink++;
  vga.printSRAM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, blinker, x, y, (toggle ? 11 : 0));
  vga.delay(17);
  vga.setExtendedColorsMask(0);

  //x++;
  // if (x==VGAX_WIDTH)
  //   x=-VGAX_WIDTH;
}