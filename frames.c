#include "Arduino.h"

int8_t const frame_0[] PROGMEM = {
   3,             // tile count
   2,  0, -7,     // tile id, x, y 
  32, -8,  0,
  33,  0,  0
};

int8_t const frame_1[] PROGMEM = {
   3,             // tile count
   2,  -1, -7,     // tile id, x, y 
  34, -8,  0,
  35,  0,  0
};

int8_t const frame_2[] PROGMEM = {
   3,             // tile count
   2,  1, -7,     // tile id, x, y 
  36, -8,  0,
  37,  0,  0
};

int8_t* const frame_list[] = {
  frame_0, frame_1, frame_2
};
