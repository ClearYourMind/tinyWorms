#include "model.h"

Model::Model(uint8_t* model, uint8_t _vertex_count) {
  uint8_t _o = pgm_read_byte(model);
  uint8_t center_x = _o >> 4;
  uint8_t center_y = _o & 0x0F;

  vertex_count = _vertex_count;
  f_vertex_x = new int16_t[vertex_count];
  f_vertex_y = new int16_t[vertex_count];

  uint8_t _v;
  for (uint8_t i=0; i<vertex_count; i++) {
    _v = pgm_read_byte(model + i + 1);
    f_vertex_x[i] = (_v >> 4) << FBITS;
    f_vertex_y[i] = (_v & 0x0F) << FBITS;
    f_vertex_x[i] -= center_x << FBITS;
    f_vertex_y[i] -= center_y << FBITS;
  }

  _scale = 1 << FBITS;
}


Model::~Model() {
  delete [] f_vertex_x;
  delete [] f_vertex_y;
}


void Model::transform_vertex(int16_t vx, int16_t vy, int16_t *x, int16_t *y) {
  *x = ((fmul(vx, _cos) - fmul(vy, _sin)) >> FBITS) + _tx;
  *y = ((fmul(vx, _sin) + fmul(vy, _cos)) >> FBITS) + _ty;
}


void Model::drawFill(int32_t x, int32_t y, Camera camera, uint8_t angle_sec, int16_t scale, uint8_t color=WHITE) {
  // check if center is off screen
  if ((x - camera.x < 0) || (x - camera.x > (uint16_t)F_WIDTH)) return;
  if ((y - camera.y < 0) || (y - camera.y > (uint16_t)F_HEIGHT)) return;

  _angle = angle_sec;
  _scale = scale;
  _tx = (x >> FBITS) - (camera.x >> FBITS);
  _ty = (y >> FBITS) - (camera.y >> FBITS);
  getSinCos(_angle, &_sin, &_cos);
  _sin = fmul(_sin, _scale);
  _cos = fmul(_cos, _scale);

  int16_t _x, _y, _x0, _y0, _x1, _y1;
  
  transform_vertex(f_vertex_x[0], f_vertex_y[0], &_x0, &_y0);
  transform_vertex(f_vertex_x[1], f_vertex_y[1], &_x1, &_y1);

  for (uint8_t i = 2; i < vertex_count; i++) {
    transform_vertex(f_vertex_x[i], f_vertex_y[i], &_x, &_y);
    arduboy.fillTriangle(_x0, _y0, _x1, _y1, _x, _y, color);

    _x0 = _x1;
    _y0 = _y1;
    _x1 = _x;
    _y1 = _y;
  }
}


void Model::drawOutline(int32_t x, int32_t y, Camera camera, uint8_t angle_sec, int16_t scale, uint8_t color=BLACK) {
  // check if center is off screen
  if ((x - camera.x < 0) || (x - camera.x > (uint16_t)F_WIDTH)) return;
  if ((y - camera.y < 0) || (y - camera.y > (uint16_t)F_HEIGHT)) return;

  _angle = angle_sec;
  _scale = scale;
  _tx = (x >> FBITS) - (camera.x >> FBITS);
  _ty = (y >> FBITS) - (camera.y >> FBITS);
  getSinCos(_angle, &_sin, &_cos);
  _sin = fmul(_sin, _scale);
  _cos = fmul(_cos, _scale);

  int16_t _x0, _y0, _x1, _y1, _x2, _y2, _xn, _yn;

  transform_vertex(f_vertex_x[0], f_vertex_y[0], &_x0, &_y0);
  transform_vertex(f_vertex_x[vertex_count-1], f_vertex_y[vertex_count-1], &_xn, &_yn);

  for (uint8_t init_i = 1; init_i < 3; init_i++) {
    _x1 = _x0;
    _y1 = _y0;
    for (uint8_t i = init_i; i < vertex_count - 1; i+=2) {
      transform_vertex(f_vertex_x[i], f_vertex_y[i], &_x2, &_y2);
      arduboy.drawLine(_x1, _y1, _x2, _y2, color);

      _x1 = _x2;
      _y1 = _y2;
      // arduboy.setCursor(0,0); arduboy.print(i);
      // arduboy.display();
      // arduboy.waitNoButtons();
      // while (!arduboy.anyPressed(255));
    }
    arduboy.drawLine(_x1, _y1, _xn, _yn, color);

  }
}
