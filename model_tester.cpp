#include "Arduboy2.h"
#include "Arduboy2Core.h"
#include "model_tester.h"

ModelTester::ModelTester() {
  scale = 1 << FBITS;
  models = new Model* [MAXMODELS];
  model_count = 0;
  focused_model_no = 0;
  button_timer = 0;
  drawing_mode = 0;
  for (uint8_t i=0; i<MAXMODELS; i++)
    model_x[i] = (i * 48 + 48) << FBITS;

  focus_x = model_x[0];
  focus_y = 48 << FBITS;
}


ModelTester::~ModelTester() {
  for (uint8_t i=0; i<model_count; i++)
    delete models[i];

  delete[] models;
  delete[] model_x;
}


bool ModelTester::time_passed(uint8_t time_in_frames) {
  int8_t d_time;

  if (counter < button_timer)
    d_time = (0xFFFF - button_timer) + counter;
  else
    d_time = counter - button_timer;

  button_timer = counter;
  return (d_time >= time_in_frames);
}


void ModelTester::add_model(uint8_t* _model_arr, uint8_t vertex_count) {
  if (model_count < MAXMODELS) {
    models[model_count] = new Model(_model_arr, vertex_count);
    model_count++ ;
  }
}

/* control:
  A + UP / DOWN    - change scale
  A + LEFT / RIGHT - change angle
  A + B            - change background
  A                - switch model
  A + A            - switch drawing mode
*/
void ModelTester::process() {
  if (arduboy.pressed(A_BUTTON)) {
    control_captured = true;
    if (arduboy.justPressed(A_BUTTON)) {
      if (time_passed(8))
        focused_model_no = (focused_model_no + 1) % model_count;
      else    // A + A
        drawing_mode = (drawing_mode + 1) % 4;
    }
    if (arduboy.justPressed(B_BUTTON))
      clear_color = (clear_color + 1) % 2;

    focus_x = model_x[focused_model_no];
    if (arduboy.justPressed(LEFT_BUTTON))
      angle = (angle + MAXANGLESEC - 1) % MAXANGLESEC;
    if (arduboy.justPressed(RIGHT_BUTTON))
      angle = (angle + 1) % MAXANGLESEC;
    if (arduboy.justPressed(UP_BUTTON))
      scale += (1 << (FBITS-4));
    if (arduboy.justPressed(DOWN_BUTTON))
      scale -= (1 << (FBITS-4));

  } else
    control_captured = false;
}


void ModelTester::draw(Camera camera) {
  arduboy.fillScreen(clear_color);

  for (uint8_t i = 0; i < model_count; i++) {
    switch (drawing_mode) {
      case 0:
        models[i]->drawFill(model_x[i], focus_y, camera, angle, scale);
        models[i]->drawOutline(model_x[i], focus_y, camera, angle, scale);
        break;
      case 1:
        models[i]->drawFill(model_x[i], focus_y, camera, angle, scale);
        break;
      case 2:
        models[i]->drawFill(model_x[i], focus_y, camera, angle, scale, BLACK);
        models[i]->drawOutline(model_x[i], focus_y, camera, angle, scale, WHITE);
        break;
      case 3:
        models[i]->drawFill(model_x[i], focus_y, camera, angle, scale, BLACK);
        break;
    }
  }
}
