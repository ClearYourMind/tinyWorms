#include "Arduino.h"

// stand_r
int8_t const frame_0[] PROGMEM = {
   3,             // tile count
   3,  0, -7,     // tile id, x, y 
  32, -8,  0,
  33,  0,  0
};

// walk_r
int8_t const frame_1[] PROGMEM = {
   3,             // tile count
   3,  -1, -7,     // tile id, x, y 
  34, -8,  0,
  35,  0,  0
};

int8_t const frame_2[] PROGMEM = {
   3,             // tile count
   3,  1, -7,     // tile id, x, y 
  36, -8,  0,
  37,  0,  0
};

// stand_r_u
int8_t const frame_3[] PROGMEM = {
   4,             // tile count
   4,  0, -7,     // tile id, x, y 
//  48, -8,  0,
  49,  0,  0,
  64, -8,  8,
  65,  0,  8
};

//walk_r_u
int8_t const frame_4[] PROGMEM = {
   4,             // tile count
   4,  -1, -7,     // tile id, x, y 
//  50, -8,  0,
  51,  0,  0,
  66, -8,  8,
  67,  0,  8
};

int8_t const frame_5[] PROGMEM = {
   5,             // tile count
   4,  1, -8,     // tile id, x, y 
  52, -8,  0,
  53,  0,  0,
  68, -8,  8,
  69,  0,  8
};

// stand_r_d
int8_t const frame_6[] PROGMEM = {
   3,             // tile count
   2,  0, -7,     // tile id, x, y 
  16, -8,  0,
  17,  0,  0
};

//walk_r_d
int8_t const frame_7[] PROGMEM = {
   3,             // tile count
   2,  0, -7,     // tile id, x, y 
  18, -8,  0,
  19,  0,  0
};

int8_t const frame_8[] PROGMEM = {
   3,             // tile count
   2,  1, -6,     // tile id, x, y 
  20, -8,  0,
  21,  0,  0
};


int8_t* const frame_list[] = {
  frame_0, frame_1, frame_2, frame_3, frame_4, frame_5, frame_6, frame_7, frame_8
};
