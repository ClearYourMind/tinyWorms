#include "Arduino.h"


/////// RIGHT ///////

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
   3,             // tile count
   4,  -1, -7,     // tile id, x, y 
//  50, -8,  0,
  51,  0,  0,
//  66, -8,  8,
  67,  0,  8
};

int8_t const frame_5[] PROGMEM = {
   4,             // tile count
   4,  3, -8,     // tile id, x, y 
  52, -6,  0,
  53,  2,  0,
  68, -6,  8,
//  69,  2,  8
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




/////// LEFT ////////

// stand_l
int8_t const frame_1B[] PROGMEM = {
   3,             // tile count
  12,  0, -7,     // tile id, x, y 
  46,  0,  0,
  47,  8,  0
};

// walk_l
int8_t const frame_1C[] PROGMEM = {
   3,             // tile count
  12,  1, -7,     // tile id, x, y 
  44,  0,  0,
  45,  8,  0
};

int8_t const frame_1D[] PROGMEM = {
   3,             // tile count
  12, -1, -7,     // tile id, x, y 
  42,  0,  0,
  43,  8,  0
};

// stand_l_u
int8_t const frame_1E[] PROGMEM = {
   4,             // tile count
  11,  0, -7,     // tile id, x, y 
//  48, -8,  0,
  62,  0,  0,
  78,  0,  8,
  79,  8,  8
};

//walk_l_u
int8_t const frame_1F[] PROGMEM = {
   3,             // tile count
  11,  1, -7,     // tile id, x, y 
//  50, -8,  0,
  60,  0,  0,
  76,  0,  8,
//  67,  0,  8
};

int8_t const frame_20[] PROGMEM = {
   4,             // tile count
  11, -2, -8,     // tile id, x, y 
  58, -1,  0,
  59,  7,  0,
  75,  7,  8
};

// stand_l_d
int8_t const frame_21[] PROGMEM = {
   3,             // tile count
  13,  0, -7,     // tile id, x, y 
  30,  0,  0,
  31,  8,  0
};

//walk_l_d
int8_t const frame_22[] PROGMEM = {
   3,             // tile count
  13,  0, -7,     // tile id, x, y 
  28,  0,  0,
  29,  8,  0
};

int8_t const frame_23[] PROGMEM = {
   3,             // tile count
  13, -1, -6,     // tile id, x, y 
  26,  0,  0,
  27,  8,  0
};

//jumpstart_l (walk_r(1) head lowered)
int8_t const frame_24[] PROGMEM = {
   3,             // tile count
  11,  1, -6,     // tile id, x, y 
  44,  0,  0,
  45,  8,  0
};

//            (walk_l_u(1) shifted forward and up)
int8_t const frame_25[] PROGMEM = {
   4,             // tile count
  11, -3, -10,     // tile id, x, y 
  58, -1, -2,
  59,  7, -2,
  75,  7,  6,
//  69, -1,  6
};

// jump_l
int8_t const frame_26[] PROGMEM = {
   3,             // tile count
  13,  0, -8,     // tile id, x, y 
  25,  0,  0,
  41,  0,  8
};

int8_t const frame_27[] PROGMEM = {
   3,             // tile count
  13,  0, -8,     // tile id, x, y 
  57,  0,  0,
  73,  0,  8
};

// jumpland_l
// (walk_r(1) head lowered)
int8_t const frame_28[] PROGMEM = {
   3,             // tile count
  44,  0,  1,     // tile id, x, y 
  45,  8,  1,
  14,  0, -5
};

int8_t const frame_29[] PROGMEM = {
   3,             // tile count
  44,  0,  0,     // tile id, x, y 
  45,  8,  0,
  14,  0, -5
};

int8_t const frame_2A[] PROGMEM = {
   3,             // tile count
  13,  0, -6,     // tile id, x, y 
  44,  0,  0,
  45,  8,  0
};

// slide_l
int8_t const frame_2B[] PROGMEM = {
  3,
  13, -1, -7,
  34, -8,  0,
  35,  0,  0,
};

int8_t const frame_2C[] PROGMEM = {
  3,
  13,  0, -8,
  34, -7, -1,
  35,  0, -1,
};

//slide_l_u
int8_t const frame_2D[] PROGMEM = {
  3,
  13, -1, -7,
  18, -8,  0,
  19,  0,  0,
};

int8_t const frame_2E[] PROGMEM = {
  3,
  13,  0, -8,
  18, -7, -1,
  19,  0, -1,
};

//slide_l_d
int8_t const frame_2F[] PROGMEM = {
  3,
  13,  0, -7,
  51,  0, -1,
  67,  0,  7,
};

int8_t const frame_30[] PROGMEM = {
  3,
  13, -1, -7,
  51,  0,  0,
  67,  0,  8,
};

//recover_r (low jump)
int8_t const frame_31[] PROGMEM = {
   3,             // tile count
  12,  0, -10,     // tile id, x, y 
  25,  0,  -2,
  41,  0,   6
};

int8_t const frame_32[] PROGMEM = {
   3,             // tile count
  12,  0, -11,    // tile id, x, y 
  57,  0,  -3,
  73,  0,   5
};

int8_t const frame_33[] PROGMEM = {
   3,             // tile count
  12,  0, -11,    // tile id, x, y 
  25,  0,  -3,
  41,  0,   5
};

int8_t const frame_34[] PROGMEM = {
   3,             // tile count
  12,  0, -10,    // tile id, x, y 
  57,  0,  -2,
  73,  0,   6
};

int8_t const frame_35[] PROGMEM = {
   3,             // tile count
  12,  0, -8,     // tile id, x, y 
  25,  0,  0,
  41,  0,  8
};


int8_t* const frame_list[] = {
  frame_0, frame_1, frame_2, frame_3, frame_4, frame_5, frame_6, frame_7, 
  frame_8, frame_9, frame_A, frame_B, frame_C, frame_D, frame_E, frame_F,
  frame_10, frame_11, frame_12, frame_13, frame_14, frame_15, frame_16, frame_17,
  frame_18, frame_19, frame_1A, frame_1B, frame_1C, frame_1D, frame_1E, frame_1F,
  frame_20, frame_21, frame_22, frame_23, frame_24, frame_25, frame_26, frame_27,
  frame_28, frame_29, frame_2A, frame_2B, frame_2C, frame_2D, frame_2E, frame_2F,
  frame_30, frame_31, frame_32, frame_33, frame_34, frame_35
  
};
