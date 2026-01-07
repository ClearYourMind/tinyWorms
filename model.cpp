#include "model.h"

Model::Model(int32_t _x, int32_t _y, uint8_t* model, uint8_t _vertex_count) {
  x = _x;
  y = _y;
  uint8_t _o = pgm_read_byte(model);
  uint8_t center_x = _o >> 4;
  uint8_t center_y = _o & 0x0F;

  vertex_count = _vertex_count;
  f_vertex_x = new int16_t[vertex_count];
  f_vertex_y = new int16_t[vertex_count];

  arduboy.fillScreen(0);
  arduboy.setCursor(16, 0); arduboy.print(vertex_count);

  uint8_t _v;
  for (uint8_t i=0; i<vertex_count; i++) {
    _v = pgm_read_byte(model + i + 1);
    f_vertex_x[i] = (_v >> 4) << FBITS;
    f_vertex_y[i] = (_v & 0x0F) << FBITS;
    f_vertex_x[i] -= center_x << FBITS;
    f_vertex_y[i] -= center_y << FBITS;
    arduboy.drawPixel((f_vertex_x[i] + x) >> FBITS, (f_vertex_y[i] + y) >> FBITS);
  }

  arduboy.display();
  arduboy.waitNoButtons();
  while (!arduboy.anyPressed(255));
}


Model::~Model() {
  delete [] f_vertex_x;
  delete [] f_vertex_y;
}


void Model::drawFill(Camera camera, uint8_t color=WHITE) {
  // check if center is off screen
  if ((x - camera.x < 0) || (x - camera.x > (uint16_t)F_WIDTH)) return;
  if ((y - camera.y < 0) || (y - camera.y > (uint16_t)F_HEIGHT)) return;

  int32_t _x, _y, _x0, _y0, _x1, _y1;
  _x0 = (f_vertex_x[0] >> FBITS) - (camera.x >> FBITS) + (x >> FBITS);
  _y0 = (f_vertex_y[0] >> FBITS) - (camera.y >> FBITS) + (y >> FBITS);
  _x1 = (f_vertex_x[1] >> FBITS) - (camera.x >> FBITS) + (x >> FBITS);
  _y1 = (f_vertex_y[1] >> FBITS) - (camera.y >> FBITS) + (y >> FBITS);

  for (uint8_t i = 2; i < vertex_count; i++) {
    _x = (f_vertex_x[i] >> FBITS) - (camera.x >> FBITS) + (x >> FBITS);
    _y = (f_vertex_y[i] >> FBITS) - (camera.y >> FBITS) + (y >> FBITS);
    arduboy.fillTriangle(_x0, _y0, _x1, _y1, _x, _y, color);

    _x0 = _x1;
    _y0 = _y1;
    _x1 = _x;
    _y1 = _y;
  }
}


void Model::drawOutline(Camera camera, uint8_t color=BLACK) {
  // check if center is off screen
  if ((x - camera.x < 0) || (x - camera.x > (uint16_t)F_WIDTH)) return;
  if ((y - camera.y < 0) || (y - camera.y > (uint16_t)F_HEIGHT)) return;

  int32_t _x0, _y0, _x1, _y1, _x2, _y2, _xn, _yn;
  _x0 = (f_vertex_x[0] >> FBITS) - (camera.x >> FBITS) + (x >> FBITS);
  _y0 = (f_vertex_y[0] >> FBITS) - (camera.y >> FBITS) + (y >> FBITS);
  _xn = (f_vertex_x[vertex_count-1] >> FBITS) - (camera.x >> FBITS) + (x >> FBITS);
  _yn = (f_vertex_y[vertex_count-1] >> FBITS) - (camera.y >> FBITS) + (y >> FBITS);

  for (uint8_t init_i = 2; init_i < 4; init_i++) {
    _x1 = _x0;
    _y1 = _y0;
    for (uint8_t i = init_i; i < vertex_count - 1; i+=2) {
      _x2 = (f_vertex_x[i] >> FBITS) - (camera.x >> FBITS) + (x >> FBITS);
      _y2 = (f_vertex_y[i] >> FBITS) - (camera.y >> FBITS) + (y >> FBITS);
      arduboy.drawLine(_x1, _y1, _x2, _y2, color);

      _x1 = _x2;
      _y1 = _y2;
    }
    arduboy.drawLine(_x1, _y1, _xn, _yn, color);
  }
}

void Model::transform(Matrix m) {
  // void transform_vertex(unsigned char v, int16_t *_x, int16_t *_y) {
  // int32_t vx = ((v >> 4) - cx) << FBITS;
  // int32_t vy = ((v & 0x0F) - cy) << FBITS;
  // *_x = (fmul(vx, m.m_cos) + fmul(vy, -m.m_sin)) + m.m_tx;
  // *_y = (fmul(vx, m.m_sin) + fmul(vy,  m.m_cos)) + m.m_ty;
  // *_x = ftoint(*_x);
  // *_y = ftoint(*_y);


}

/*
void m_calc(mx *m)  {
  int32_t s, c;
  getSinCos(m->angle_sec, &s, &c); // try not using pointers (less code?)
  m->m_sin = fmul(fmul(s,  m->m_scale), cam.scale);
  m->m_cos = fmul(fmul(c,  m->m_scale), cam.scale);
  m->m_tx = fmul(m->m_x - cam.x, cam.scale) + (cam.cx<<FBITS);
  m->m_ty = fmul(m->m_y - cam.y, cam.scale) + (cam.cy<<FBITS);
};


  private:
  void transform_vertex(unsigned char v, int16_t *_x, int16_t *_y) {
    int32_t vx = ((v >> 4) - cx) << FBITS;
    int32_t vy = ((v & 0x0F) - cy) << FBITS;
    *_x = (fmul(vx, m.m_cos) + fmul(vy, -m.m_sin)) + m.m_tx;
    *_y = (fmul(vx, m.m_sin) + fmul(vy,  m.m_cos)) + m.m_ty;
    *_x = ftoint(*_x);
    *_y = ftoint(*_y);

  };
  s  */
