#include "camera.h"
#include "common.h"

void Camera::process() {
  int16_t dx; // fixed
  int16_t dy; // fixed 

  dx = ((max(focus_x, -F_WIDTH_H) + F_WIDTH_H) - x) >> 4; // fraction of x and focus_x difference
  dy = ((max(focus_y, F_HEIGHT_H) - F_HEIGHT_H) - y) >> 4;

  x += dx;
  y += dy;

  cell_x = (x >> FBITS) >> 2;
  cell_y = (y >> FBITS) >> 2;

}


void Camera::processControls() {
  if (arduboy.pressed(RIGHT_BUTTON)) {
    focus_x += (2 << FBITS);
  };
  if (arduboy.pressed(LEFT_BUTTON)) {
    focus_x -= (2 << FBITS);
  };
  if (arduboy.pressed(UP_BUTTON)) {
    focus_y -= (2 << FBITS);
  };
  if (arduboy.pressed(DOWN_BUTTON)) {
    focus_y += (2 << FBITS);
  };
}


void Camera::drawDebugOverlay() {
  uint16_t _x;
  uint16_t _y;
  uint16_t _fx;
  uint16_t _fy;
  uint8_t osc = counter % 2;

  _x = x >> FBITS;
  _y = y >> FBITS;
  _fx = focus_x >> FBITS;
  _fy = focus_y >> FBITS;
  arduboy.drawRect(_fx - 2, _fy - 2, 4, 4, osc);
  arduboy.drawRect(_x, _y, 128, 64, osc);
  arduboy.drawRect(cell_x << 2, cell_y << 2, 7, 7, osc);

}