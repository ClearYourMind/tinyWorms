#ifndef FIXEDMATH_H
#define FIXEDMATH_H

#include <stdint.h>
#include <avr/pgmspace.h>
#define SCPREC 8                  // sectors in quadrant (precision)
#define SCPREC_LOG2 3             // power of 2 sectors in a quadrant (2^3 = 8)
#define MAXANGLESEC 32 //SCPREC * 4
#define FBITS 8

//// Fixed point math //////
inline int32_t fmul(int32_t A, int32_t B) {
	return (A * B) >> FBITS;
}

inline int16_t ftoint(int32_t A) {
  return A >> FBITS;
}
////////////////////////////

////   Sin/Cos table  //////
const uint8_t scTab[SCPREC + 1] PROGMEM = {
  0, 50, 98, 142, 180, 212, 236, 250, 255
};

inline void getSinCos(uint8_t sec, int32_t *s, int32_t *c) {   // a - angle. must be + and <=360
  uint8_t q = sec >> SCPREC_LOG2;

  switch (q) {
  case 0:
    *s = pgm_read_byte_near(scTab + sec % SCPREC);
    *c = pgm_read_byte_near(scTab + SCPREC - sec % SCPREC);
  break;
  case 1:
    *s =  pgm_read_byte_near(scTab + SCPREC - sec % SCPREC);
    *c = -pgm_read_byte_near(scTab + sec % SCPREC);
  break;
  case 2:
    *s = -pgm_read_byte_near(scTab + sec % SCPREC);
    *c = -pgm_read_byte_near(scTab + SCPREC - sec % SCPREC);
  break;
  case 3:
    *s = -pgm_read_byte_near(scTab + SCPREC - sec % SCPREC);
    *c =  pgm_read_byte_near(scTab + sec % SCPREC);
  break;
  };
}
////////////////////////////

#endif
