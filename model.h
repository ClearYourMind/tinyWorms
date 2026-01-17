#ifndef MODEL_H
#define MODEL_H

#include "Arduboy2.h"
#include "fixedmath.h"
#include "camera.h"

extern Arduboy2 arduboy;
//extern void debug_stop(int32_t val_1, int32_t val_2, const char message[] = NULL);

class Model {
  private:
    uint8_t center_x;
    uint8_t center_y;
    uint8_t* model;
  public:
    int16_t* f_vertex_x;
    int16_t* f_vertex_y;
    uint8_t vertex_count;
    int32_t x;
    int32_t y;

    void transform(uint8_t angle_sec, int16_t scale);
    void drawFill(int8_t x, int8_t y, uint8_t color=WHITE);
    void drawOutline(int8_t x, int8_t y, uint8_t color=BLACK);
    void drawDots(int8_t x, int8_t y, uint8_t color=BLACK);

    Model(uint8_t* model, uint8_t _vertex_count);
    ~Model();
};

#endif