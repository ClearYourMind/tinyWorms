#ifndef MODEL_TESTER
#define MODEL_TESTER

#include "Arduboy2.h"
#include "fixedmath.h"
#include "camera.h"
#include "model.h"

#define MAXMODELS 8

extern Arduboy2 arduboy;
extern uint16_t counter;


class ModelTester {
  private:
    uint8_t angle = 0;
    int16_t scale = 1 << FBITS;
    Model** models;
    uint8_t focused_model_no;
    int8_t drawing_mode;
    bool frames_passed(uint8_t time_in_frames);
  public:
    int16_t focus_x;
    int16_t focus_y;
    uint8_t model_count;
    int32_t model_x[MAXMODELS];
    bool control_captured;
    uint8_t clear_color = WHITE;

    ModelTester();
    ~ModelTester();
    void process();
    void draw(Camera camera);
    void add_model(uint8_t* _model_arr, uint8_t vertex_count);
};


#endif