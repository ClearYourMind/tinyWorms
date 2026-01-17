#include "model_tester.h"

ModelTester::ModelTester() {
  scale = 1 << FBITS;
  models = new Model* [MAXMODELS];
  model_count = 0;
  focused_model_no = 0;
  drawing_mode = 4;
  background_mode = 0;    // 0..1 - b/w only; 2..3 - b/w + sprite
  for (uint8_t i=0; i<MAXMODELS; i++)
    model_x[i] = (i * 48 + 48) << FBITS;

  focus_x = model_x[0];
  focus_y = 32 << FBITS;
}


ModelTester::~ModelTester() {
  for (uint8_t i=0; i<model_count; i++)
    delete models[i];

  delete[] models;
  delete[] model_x;
}


bool ModelTester::frames_passed(uint8_t time_in_frames) {
  static uint16_t last_counter;
  uint16_t d_time;

  d_time = counter - last_counter;

  last_counter = counter;
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
      if (frames_passed(8))
        focused_model_no = (focused_model_no + 1) % model_count;
      else    // A + A
        drawing_mode = (drawing_mode + 1) % 5;
    }
    if (arduboy.justPressed(B_BUTTON))
      background_mode = (background_mode + 1) % 4;

    focus_x = model_x[focused_model_no];
    if (arduboy.justPressed(LEFT_BUTTON))
      angle = (angle + MAXANGLESEC - 1) % MAXANGLESEC;
    if (arduboy.justPressed(RIGHT_BUTTON))
      angle = (angle + 1) % MAXANGLESEC;
    if (arduboy.justPressed(UP_BUTTON))
      scale += 1;
    if (arduboy.justPressed(DOWN_BUTTON))
      scale -= 1;

  } else
    control_captured = false;
}


void ModelTester::draw(Camera camera) {
  // background
  uint8_t origin_color = counter % 2;
  int8_t origin_x;
  int8_t origin_y = (focus_y >> FBITS) - (camera.y >> FBITS);

  arduboy.fillScreen(background_mode % 2);

  if ((background_mode >> 1) % 2) {
    for (uint8_t i=0; i<model_count; i++) {
      origin_x = (model_x[i] >> FBITS) - (camera.x >> FBITS);
      drawFrame(origin_x, origin_y, 0);
    }
  }

  // model
  for (uint8_t i = 0; i < model_count; i++) {
    origin_x = (model_x[i] >> FBITS) - (camera.x >> FBITS);
    switch (drawing_mode) {
      case 0:
        models[i]->drawFill(origin_x, origin_y, angle, scale);
        models[i]->drawOutline(origin_x, origin_y, angle, scale);
        break;
      case 1:
        models[i]->drawFill(origin_x, origin_y, angle, scale);
        break;
      case 2:
        models[i]->drawFill(origin_x, origin_y, angle, scale, BLACK);
        models[i]->drawOutline(origin_x, origin_y, angle, scale, WHITE);
        break;
      case 3:
        models[i]->drawFill(origin_x, origin_y, angle, scale, BLACK);
        break;
      case 4:
        arduboy.drawRect(origin_x-1, origin_y-1, 18, 18, 1 - origin_color);
        models[i]->drawDots(origin_x, origin_y, angle, scale, WHITE); //counter % 2);
        break;
    }
    // draw origin coord
    //if (drawing_mode != 4) {
      arduboy.drawPixel(origin_x - 1, origin_y, origin_color);
      arduboy.drawPixel(origin_x + 1, origin_y, origin_color);
      arduboy.drawPixel(origin_x, origin_y - 1, origin_color);
      arduboy.drawPixel(origin_x, origin_y + 1, origin_color);
    //}
  }

  arduboy.setCursor(0, 0); arduboy.print("scale:");arduboy.print(scale);
  arduboy.setCursor(0, 8); arduboy.print("angle:");arduboy.print(angle);
  
}
