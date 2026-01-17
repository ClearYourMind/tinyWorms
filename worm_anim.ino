#include "Arduboy2.h"

#include "common.h"

Arduboy2 arduboy;
Sprites sprites;

#include "camera.h"
#include "fixedmath.h"
#include "model.h"
#include "models.h"
#include "terrain_generator.h"
#include "player.h"

uint16_t counter = 0;
bool debug_info_toggle = false;

uint16_t screenNo = 0;
Camera camera;
Player player;
TerrainGenerator terrain_gen(42);

void debug_stop(int32_t val_1, int32_t val_2, const char message[] = NULL) {
  arduboy.fillScreen(0);
  arduboy.setTextColor(WHITE);
  arduboy.setCursor(32, 0);
  arduboy.print("DEBUG STOP");

  arduboy.setCursor(32, 16);
  arduboy.print(message);
  arduboy.setCursor(32, 40);
  arduboy.print(val_1);
  arduboy.setCursor(80, 40);
  arduboy.print(val_2);
  arduboy.display();
  arduboy.waitNoButtons();
  while (!arduboy.anyPressed(255));

}

void debug_stop(int32_t value, const char message[] = NULL) {
  debug_stop(value, 0, message);
}


uint8_t setFlagAsBool(uint8_t flags, uint8_t flag, bool bool_value) {
  return bool_value ? (flags | flag) : (flags & ~((uint8_t)flag));
}


void setCell(uint32_t *field[], uint8_t x, uint8_t y, bool value) {
  uint8_t f_screen; /// 0..3
  f_screen = (x >> 5) % 2;            //  (cx / 32) mod 2 = 0..1
  f_screen += ((y >> 4) % 2) << 1;    // ((cy / 16) mod 2) * 2 = 0..2
  uint32_t *_field = field[f_screen];
  x = x % CELL_COUNT_X;
  y = y % CELL_COUNT_Y;
  if (value)
    _field[y] |= (0x80000000 >> x);
  else
    _field[y] &= ~(0x80000000 >> x);
}


bool getCell(uint32_t *field[], uint8_t x, uint8_t y) {
  uint8_t f_screen; /// 0..3
  f_screen = (x >> 5) % 2;            //  (cx / 32) mod 2 = 0..1
  f_screen += ((y >> 4) % 2) << 1;    // ((cy / 16) mod 2) * 2 = 0..2
  uint32_t *_field = field[f_screen];
  x = x % CELL_COUNT_X;
  y = y % CELL_COUNT_Y;
  return (_field[y] & (0x80000000 >> x)) != 0;
}


void drawField(Camera camera) {
  int8_t yy = 0;
  int8_t xx = 0;

  for (uint8_t j=camera.cell_y; j<camera.cell_y + CELL_COUNT_Y; j++)
  for (uint8_t i=camera.cell_x; i<camera.cell_x + CELL_COUNT_X; i++) {
    if (getCell(field_ptr, i, j)) {
      xx = (i << 2) - (camera.x >> FBITS);
      yy = (j << 2) - (camera.y >> FBITS);
      sprites.drawErase(xx, yy, ground, 0);
    }
  }
}


void screenOverlap() {
  uint32_t *t_ptr;

  t_ptr = field_ptr[0];
  field_ptr[0] = field_ptr[1];
  field_ptr[1] = t_ptr;
  t_ptr = field_ptr[2];
  field_ptr[2] = field_ptr[3];
  field_ptr[3] = t_ptr;

  camera.x -= F_WIDTH;
  camera.focus_x -= F_WIDTH;
  player.x -= F_WIDTH;

  screenNo++;
  // generate terrain for field_ptr[1][3]
  terrain_gen.generateScreen(field_ptr[1], field_ptr[3], field_ptr[0], field_ptr[2]);
}


void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);
  player.x = 110 << FBITS;
  player.y = 40 << FBITS;
}


void loop() {
  if (!(arduboy.nextFrameDEV()))
    return;

  arduboy.pollButtons();

  counter++;

  if (arduboy.justPressed(B_BUTTON)) {
    debug_info_toggle = !debug_info_toggle;
    arduboy.setFrameRate(debug_info_toggle ? 20 : 30);
  }

  // check overscroll
  if (camera.x > F_WIDTH)
    screenOverlap();

  player.process();
  player.processAnim();

  camera.focus_x = player.x;
  camera.focus_y = player.y;
  camera.process();

  drawField(camera);
  if (debug_info_toggle)
    camera.drawDebugOverlay();

  arduboy.display();
  arduboy.idle();
}
