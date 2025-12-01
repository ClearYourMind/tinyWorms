#ifndef CAMERA_H
#define CAMERA_H

#include "Arduino.h"
#include "fixedmath.h"
#include "Arduboy2.h"

#define F_WIDTH (WIDTH << FBITS)
#define F_HEIGHT (HEIGHT << FBITS)
#define F_WIDTH_H (F_WIDTH >> 1)
#define F_HEIGHT_H (F_HEIGHT >> 1)

extern Arduboy2 arduboy;
extern uint16_t counter;

class Camera {
  public:
    int32_t x; // fixed point 
    int32_t y; // fixed point
    int8_t cell_x;
    int8_t cell_y;
    int8_t screen_x;
    int8_t screen_y;
    int32_t focus_x; // fixed point
    int32_t focus_y; // fixed point

    void process();
    void processControls();
    void drawDebugOverlay();
};

#endif
