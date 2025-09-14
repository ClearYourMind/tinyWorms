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
   4,  3, -10,     // tile id, x, y 
  52, -7, -2,
  53,  1, -2,
  68, -7,  6,
  69,  1,  6
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
  34, -8,  1,     // tile id, x, y 
  35,  0,  1,
   1,  0, -5
};

int8_t const frame_E[] PROGMEM = {
   3,             // tile count
  34, -8,  0,     // tile id, x, y 
  35,  0,  0,
   1,  0, -5
};

int8_t const frame_F[] PROGMEM = {
   3,             // tile count
   2,  0, -6,     // tile id, x, y 
  34, -8,  0,
  35,  0,  0
};

// slide_r
int8_t const frame_10[] PROGMEM = {
  3,
  2,  1, -7,
  44, 0,  0,
  45, 8,  0,
};

int8_t const frame_11[] PROGMEM = {
  3,
  2,  0, -8,
  44, 0,  -1,
  45, 7,  -1,
};

//slide_r_u
int8_t const frame_12[] PROGMEM = {
  3,
  2,  1, -7,
  28, 0,  0,
  29, 8,  0,
};

int8_t const frame_13[] PROGMEM = {
  3,
  2,  0, -8,
  28, 0,  -1,
  29, 7,  -1,
};

//slide_r_d
int8_t const frame_14[] PROGMEM = {
  3,
  2,  0, -7,
  60, 0, -1,
  76, 0,  7,
};

int8_t const frame_15[] PROGMEM = {
  3,
  2,  1, -7,
  60, 0,  0,
  76, 0,  8,
};

//recover_r (low jump)
int8_t const frame_16[] PROGMEM = {
   3,             // tile count
   3,  0, -10,     // tile id, x, y 
  22,  0,  -2,
  38,  0,   6
};

int8_t const frame_17[] PROGMEM = {
   3,             // tile count
   3,  0, -11,    // tile id, x, y 
  54,  0,  -3,
  70,  0,   5
};

int8_t const frame_18[] PROGMEM = {
   3,             // tile count
   3,  0, -11,    // tile id, x, y 
  22,  0,  -3,
  38,  0,   5
};

int8_t const frame_19[] PROGMEM = {
   3,             // tile count
   3,  0, -10,    // tile id, x, y 
  54,  0,  -2,
  70,  0,   6
};

int8_t const frame_1A[] PROGMEM = {
   3,             // tile count
   3,  0, -8,     // tile id, x, y 
  22,  0,  0,
  38,  0,  8
};

int8_t* const frame_list[] = {
  frame_0, frame_1, frame_2, frame_3, frame_4, frame_5, frame_6, frame_7, 
  frame_8, frame_9, frame_A, frame_B, frame_C, frame_D, frame_E, frame_F,
  frame_10, frame_11, frame_12, frame_13, frame_14, frame_15, frame_16, frame_17,
  frame_18, frame_19, frame_1A
};
