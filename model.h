#ifndef MODEL_H
#define MODEL_H

#include "Arduboy2.h"
#include "fixedmath.h"
#include "camera.h"

extern Arduboy2 arduboy;

struct Matrix {
  int16_t sin;
  int16_t cos;
  int16_t scale;
  // int32_t x;  // original position on scene
  // int32_t y;
  // int32_t tx; // position on screen
  // int32_t ty;
  uint8_t angle_sec;
};

class Model {
  public:
    int16_t* f_vertex_x;
    int16_t* f_vertex_y;
    uint8_t vertex_count;
    // mx matrix;
    // uint8_t center_x;
    // uint8_t center_y;
    int32_t x;
    int32_t y;

    void transform(Matrix m);
    void drawFill(Camera camera, uint8_t color=WHITE);
    void drawOutline(Camera camera, uint8_t color=BLACK);
    Model(int32_t _x, int32_t _y, uint8_t* model, uint8_t _vertex_count);
    ~Model();
};

#endif