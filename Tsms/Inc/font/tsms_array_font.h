#ifndef TSMS_ARRAY_FONT_H
#define TSMS_ARRAY_FONT_H

// width by height
// ASCII font file : DEFAULT 5 by 8 
// Define the ASCII table as Data array
// cols left to right 0x00 is  off 0xFF is all on
// Each character is five bytes wide one byte height. 
// so ! is 0 0 5F 0 0 , Also in code adds two blank lines on either side so in effect 0 0 0 5f 0 0 0 
// Each character is in a 7 by 8 pixel box 

extern const unsigned char TSMS_FONT_DEFAULT_FONT[94][7];
#define TSMS_FONT_DEFAULT_FONT_WIDTH 7
#define TSMS_FONT_DEFAULT_FONT_HEIGHT 8

// ASCII font file : Thick Font 7 by 8 
// Define the ASCII table as Data array
// cols left to right 0x00 is  off 0xFF is all on
// Each character is 7 bytes wide one byte height. 
// so ! is 0x5f,0x5f,0x00,0x00,0x00,0x00,0x00,0x00 , 
// Also the code adds two blank lines on LHS and RHS side 
// Each character is in a 9 by 8 bit pixel box.

// Q: what is lhs?
// A: left hand side
// Q: so ! in pixel 9 * 8
// A
extern const unsigned char TSMS_FONT_THICK_FONT[59][9];
#define TSMS_FONT_THICK_FONT_WIDTH 9
#define TSMS_FONT_THICK_FONT_HEIGHT 8



// ASCII font file : Homespun FONT 7 by 8 
// added v2.3
extern const unsigned char TSMS_FONT_HOME_SPUN_FONT[95][7];
#define TSMS_FONT_HOME_SPUN_FONT_WIDTH 7
#define TSMS_FONT_HOME_SPUN_FONT_HEIGHT 8


// ASCII font file : Seven Segment 4 by 8 
// Define the ASCII table as Data array
// cols left to right 0x00 is  off 0xFF is all on
// Each character is five bytes wide one byte height. 
// so ! is 0x36 0 0  0 0 , Also in code adds two blank lines on either side so in effect 0 36 0 0 0 0 0 
// Each character is in a 6 by 8 pixel box
extern const unsigned char TSMS_FONT_SEVEN_SEG_FONT[92][6];
#define TSMS_FONT_SEVEN_SEG_FONT_WIDTH 6
#define TSMS_FONT_SEVEN_SEG_FONT_HEIGHT 8


// ASCII font file : Number five Wide font 8 by 8 
// Define the ASCII table as Data array
// cols left to right 0x00 is  off 0xFF is all on 
// Each character is in a 10 by 8 bit pixel box
extern const unsigned char TSMS_FONT_WIDE_FONT[59][10];
#define TSMS_FONT_WIDE_FONT_WIDTH 10
#define TSMS_FONT_WIDE_FONT_HEIGHT 8


// ASCII font file : tiny 3 by 8 
// Define the ASCII table as Data array
// cols left to right, 0x00 is off, 0xFF is all on
// Each character is 3 bytes wide one byte height. 
// Each character is in a 5 by 8 pixel box ( two blanks bytes on either side added in code)
extern const unsigned char TSMS_FONT_TINY_FONT[95][5];
#define TSMS_FONT_TINY_FONT_WIDTH 5
#define TSMS_FONT_TINY_FONT_HEIGHT 8


// ASCII font file : "large" 12 by 16 
// Define the ASCII table as Data array
// cols left to right, 0x00 is off, 0xFF is all on
// Each character is 12 bits wide, two byte height.
// NO LOWERCASE LETTERS
extern const uint16_t TSMS_FONT_LARGE_FONT[59][12];
#define TSMS_FONT_LARGE_FONT_WIDTH 12
#define TSMS_FONT_LARGE_FONT_HEIGHT 16


// ASCII font file : huge  16 by 24 
// Define the ASCII table as Data array
// cols left to right, 0x00 is off, 0xFF is all on
// Each character is 16 bits wide,  3 byte height. 
//Numbers + . : only. type / for a space
extern const uint32_t TSMS_FONT_HUGE_FONT[13][16];
#define TSMS_FONT_HUGE_FONT_WIDTH 16
#define TSMS_FONT_HUGE_FONT_HEIGHT 24


// ASCII font file : Mega 16 by 32 
// Define the ASCII table as Data array
// cols left to right, 0x00 is off, 0xFF is all on
// Each character is 16 bits wide 4 byte height. 
// Numbers only + : . /
extern const uint32_t TSMS_FONT_MEGA_FONT[13][16];
#define TSMS_FONT_MEGA_FONT_WIDTH 16
#define TSMS_FONT_MEGA_FONT_HEIGHT 32

#endif	// TSMS_ARRAY_FONT_H