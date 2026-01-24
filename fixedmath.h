#ifndef FIXEDMATH_H
#define FIXEDMATH_H

#include <stdint.h>
#include <avr/pgmspace.h>
#define MAXANGLESEC 32 //SCPREC * 4
#define FBITS 6

//// Fixed point math //////
inline int32_t fmul(int32_t A, int32_t B) {
	return (A * B) >> FBITS;
}

////////////////////////////

////   Sin/Cos table  //////
const int8_t sTab[MAXANGLESEC] PROGMEM = {
  0,  12,  24,  35,  45,  53,  59,  62,  64,  62,  59,  53,  45,  35,  24,  12,
  0, -12, -24, -35, -45, -53, -59, -62, -64, -62, -59, -53, -45, -35, -24, -12
};
const int8_t cTab[MAXANGLESEC] PROGMEM = {
   64,  62,  59,  53,  45,  35,  24,  12, 0, -12, -24, -35, -45, -53, -59, -62,
  -64, -62, -59, -53, -45, -35, -24, -12, 0,  12,  24,  35,  45,  53,  59,  62
};

inline void getSinCos(uint8_t sec, int16_t *s, int16_t *c) {  // sec must be <MAXANGLESEC
  *s = (int8_t)pgm_read_byte_near(sTab + sec);
  *c = (int8_t)pgm_read_byte_near(cTab + sec);
}
////////////////////////////

#endif
