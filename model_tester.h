#ifndef MODEL_TESTER
#define MODEL_TESTER

#include "Arduboy2.h"
#include "fixedmath.h"
#include "camera.h"
#include "model.h"

#define MAXMODELS 8

extern Arduboy2 arduboy;


class ModelTester {
  private:
    uint8_t angle = 0;
    int16_t scale = 1 << FBITS;
    uint8_t clear_color = WHITE;
    Model** models;
    int32_t model_x[MAXMODELS];
    //int32_t model_y[MAXMODELS];
    uint8_t model_count;
    uint8_t focused_model_no;
  public:
    int16_t focus_x;
    int16_t focus_y;
    bool control_captured;

    ModelTester();
    ~ModelTester();
    void process();
    void draw(Camera camera);
    void add_model(uint8_t* _model_arr, uint8_t vertex_count);
};


#endif