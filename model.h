#ifndef MODEL_H
#define MODEL_H

#include "Arduboy2.h"
#include "fixedmath.h"
#include "camera.h"

extern Arduboy2 arduboy;

class Model {
  private:
    int16_t _sin;
    int16_t _cos;
    uint8_t _angle;
    int16_t _scale;
    int16_t _tx;
    int16_t _ty;
    void transform_vertex(int16_t vx, int16_t vy, int16_t *x, int16_t *y);  // use pointer in param ?
  public:
    int16_t* f_vertex_x;
    int16_t* f_vertex_y;
    uint8_t vertex_count;
    //Matrix matrix;
    // uint8_t center_x;
    // uint8_t center_y;
    int32_t x;
    int32_t y;

//    void transform();
    void drawFill(int8_t x, int8_t y, uint8_t angle_sec, int16_t scale, uint8_t color=WHITE);
    void drawOutline(int8_t x, int8_t y, uint8_t angle_sec, int16_t scale, uint8_t color=BLACK);
    Model(uint8_t* model, uint8_t _vertex_count);
    ~Model();
};

#endif