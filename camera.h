#ifndef CAMERA_H
#define CAMERA_H

#include "Arduino.h"
#include "fixedmath.h"
#include "Arduboy2.h"
#include "common.h"

extern Arduboy2 arduboy;
extern uint16_t counter;

class Camera {
  public:
    int32_t x; // fixed point 
    int32_t y; // fixed point
    int8_t cell_x;
    int8_t cell_y;
    int32_t focus_x; // fixed point
    int32_t focus_y; // fixed point

    void process();
    void processControls();
    void drawDebugOverlay();

    Camera();
};

#endif
