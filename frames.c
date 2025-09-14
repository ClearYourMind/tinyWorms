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
   4,  3, -8,     // tile id, x, y 
  52, -6,  0,
  53,  2,  0,
  68, -6,  8,
  69,  2,  8
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

//jumpstart_r (walk_r(1) head lowered)
int8_t const frame_9[] PROGMEM = {
   3,             // tile count
   4, -1, -6,     // tile id, x, y 
  34, -8,  0,
  35,  0,  0
};

//            (walk_r_u(1) shifted forward and up)
int8_t const frame_A[] PROGMEM = {
   5,             // tile count
   4,  5,-12,     // tile id, x, y 
  52, -4, -4,
  53,  4, -4,
  68, -4,  4,
  69,  4,  4
};

// jump_r
int8_t const frame_B[] PROGMEM = {
   3,             // tile count
   2,  0, -8,     // tile id, x, y 
  22,  0,  0,
  38,  0,  8
};

int8_t const frame_C[] PROGMEM = {
   3,             // tile count
   2,  0, -8,     // tile id, x, y 
  54,  0,  0,
  70,  0,  8
};

// jumpland_r
// (walk_r(1) head lowered)
int8_t const frame_D[] PROGMEM = {
   3,             // tile count
  34, -8,  0,     // tile id, x, y 
  35,  0,  0,
   1,  0, -5
};

int8_t const frame_E[] PROGMEM = {
   3,             // tile count
   2,  0, -6,     // tile id, x, y 
  34, -8,  0,
  35,  0,  0
};

int8_t const frame_F[] PROGMEM = {
};


int8_t* const frame_list[] = {
  frame_0, frame_1, frame_2, frame_3, frame_4, frame_5, frame_6, frame_7, 
  frame_8, frame_9, frame_A, frame_B, frame_C, frame_D, frame_E, frame_F
};
