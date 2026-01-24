#include "pins_arduino.h"
#include "model.h"

Model::Model(uint8_t* _model, uint8_t _vertex_count) {
  model = _model;
  uint8_t _o = pgm_read_byte(_model);
  center_x = _o >> 4;
  center_y = _o & 0x0F;

  vertex_count = _vertex_count;
  f_vertex_x = new int16_t[vertex_count]; // screen vertices
  f_vertex_y = new int16_t[vertex_count];

  uint8_t _v;
  for (uint8_t i=0; i<vertex_count; i++) {
    _v = pgm_read_byte(_model + i + 1);
    f_vertex_x[i] = (_v >> 4);
    f_vertex_y[i] = (_v & 0x0F);
    f_vertex_x[i] -= center_x;
    f_vertex_y[i] -= center_y;
  }
}


Model::~Model() {
  delete [] f_vertex_x;
  delete [] f_vertex_y;
}


void Model::transform(uint8_t angle_sec, int16_t scale) {
  int16_t _s, _c;
  getSinCos(angle_sec, &_s, &_c);
  _s = _s * scale;
  _c = _c * scale;

  uint8_t _v;
  int16_t vx, vy;
  for (uint8_t i = 0; i < vertex_count; i++) {
    _v = pgm_read_byte(model + i + 1);
    vx = (_v >> 4) - center_x;
    vy = (_v & 0x0F) - center_y;
    f_vertex_x[i] = (fmul(vx, _c) - fmul(vy, _s)) >> FBITS;
    f_vertex_y[i] = (fmul(vx, _s) + fmul(vy, _c)) >> FBITS;
  }
}


void Model::drawFill(int8_t x, int8_t y, uint8_t color=WHITE) {
  // check if center is off screen
  if ((x < 0) || (y < 0)) return;

  int16_t _x, _y, _x0, _y0, _x1, _y1;
  
  _x0 = f_vertex_x[0] + x;
  _y0 = f_vertex_y[0] + y;
  _x1 = f_vertex_x[1] + x;
  _y1 = f_vertex_y[1] + y;

  for (uint8_t i = 2; i < vertex_count; i++) {
    _x = f_vertex_x[i] + x;
    _y = f_vertex_y[i] + y;
    arduboy.fillTriangle(_x0, _y0, _x1, _y1, _x, _y, color);

    _x0 = _x1;
    _y0 = _y1;
    _x1 = _x;
    _y1 = _y;
  }
}


void Model::drawOutline(int8_t x, int8_t y, uint8_t color=BLACK) {
  // check if center is off screen
  if ((x < 0) || (y < 0)) return;

  int16_t _x1, _y1, _x2, _y2, _xn, _yn;

  _xn = f_vertex_x[vertex_count-1] + x;
  _yn = f_vertex_y[vertex_count-1] + y;

  for (uint8_t init_i = 1; init_i < 3; init_i++) {
    _x1 = f_vertex_x[0] + x;
    _y1 = f_vertex_y[0] + y;
    for (uint8_t i = init_i; i < vertex_count - 1; i+=2) {
      _x2 = f_vertex_x[i] + x;
      _y2 = f_vertex_y[i] + y;
      arduboy.drawLine(_x1, _y1, _x2, _y2, color);

      _x1 = _x2;
      _y1 = _y2;
    }
    arduboy.drawLine(_x1, _y1, _xn, _yn, color);

  }
}


void Model::drawDots(int8_t x, int8_t y, uint8_t color=BLACK) {
  // check if center is off screen
  if ((x < 0) || (y < 0)) return;

  int16_t _x, _y;

  for (uint8_t i = 0; i < vertex_count; i++) {
    _x = f_vertex_x[i] + x;
    _y = f_vertex_y[i] + y;
    arduboy.drawPixel(_x, _y, color);
  }
}
