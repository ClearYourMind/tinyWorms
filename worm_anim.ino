#include "Arduboy2.h"

#include "animations.h"
#include "common.h"

Arduboy2 arduboy;
Sprites sprites;

#include "player.h"
#include "camera.h"
#include "fixedmath.h"
#include "terrain_generator.h"

uint16_t counter = 0;
bool debug_info_toggle = false;

uint8_t const ground[] PROGMEM = {
  7, 8, 
  0x1C, 0x3E, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C
};

uint32_t field[4][CELL_COUNT_Y] = {
  {
    0x0003FFF0,
    0x0000FFC0,
    0x00000780,
    0x00000001,

    0x800C0001,
    0x803B0003,
    0xC073E07F,
    0xC0FBE007,

    0x81FBF001,
    0x87F1FC03,
    0x87FFFE03,
    0xCFC3FF1F,

    0xFF000F8F,
    0xFC000FC7,
    0x78001FE1,
    0x20003FF0
  },
  {
    0x0003FFF0,
    0x0000FFC0,
    0x00000780,
    0x00000001,

    0x800E0001,
    0x80310003,
    0xC07DE07F,
    0xC0FBE007,

    0x81F7F001,
    0x87F1FC03,
    0x87FFFE03,
    0xCFC3FF1F,

    0xFF000F8F,
    0xFC000FC7,
    0x78001FE1,
    0x20003FF0
  },
  {
    0x0003FFF0,
    0x0000FFC0,
    0x00000780,
    0x00000001,

    0x800C0001,
    0x80330003,
    0xC079E07F,
    0xC0F3E007,

    0x81F9F001,
    0x87F3FC03,
    0x87FFFE03,
    0xCFC3FF1F,

    0xFF000F8F,
    0xFC000FC7,
    0x78001FE1,
    0x20003FF0
  },
  {
    0x0003FFF0,
    0x0000FFC0,
    0x00000780,
    0x00000001,

    0x800C0001,
    0x80330003,
    0xC06BE07F,
    0xC0EBE007,

    0x81E1F001,
    0x87FBFC03,
    0x87FFFE03,
    0xCFC3FF1F,

    0xFF000F8F,
    0xFC000FC7,
    0x78001FE1,
    0x20003FF0
  }
};

uint32_t *field_ptr[4] = {
  field[0],
  field[1],
  field[2],
  field[3]
};

uint16_t screenNo = 0;
Camera camera;
//Player player;
TerrainGenerator terrain_gen(42);

void debug_stop(int32_t value, const char message[] = NULL) {
  arduboy.fillScreen(0);
  arduboy.setTextColor(WHITE);
  arduboy.setCursor(32, 8);
  arduboy.print("DEBUG STOP");

  arduboy.setCursor(32, 24);
  arduboy.print(message);
  arduboy.setCursor(48, 32);
  arduboy.print(value);

  arduboy.display();
  arduboy.waitNoButtons();
  while (!arduboy.anyPressed(255));

}


uint8_t setFlagAsBool(uint8_t flags, uint8_t flag, bool bool_value) {
  return bool_value ? (flags | flag) : (flags & ~((uint8_t)flag));
}


void setCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y, bool value) {
  if (x >= CELL_COUNT_X || y >= CELL_COUNT_Y) return false;
  if (value)
    field[y] |= (0x80000000 >> x);
  else
    field[y] &= ~(0x80000000 >> x);
}

bool getCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y) {
  return (field[y] & (0x80000000 >> x)) != 0;
}


void drawField(Camera camera) {
  int8_t yy = 0;
  int8_t xx = 0;
  uint8_t f_screen;

  for (uint8_t j=camera.cell_y; j<camera.cell_y + CELL_COUNT_Y; j++)
  for (uint8_t i=camera.cell_x; i<camera.cell_x + CELL_COUNT_X; i++) {
    f_screen = (i >> 5) % 2;            //  (cx / 32) mod 2 = 0..1
    f_screen += ((j >> 4) % 2) << 1;    // ((cy / 16) mod 2) * 2 = 0..2
    if (getCell(field_ptr[f_screen], i % CELL_COUNT_X, j % CELL_COUNT_Y)) {
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

  camera.x -= (uint16_t)F_WIDTH;
  camera.focus_x -= (uint16_t)F_WIDTH;
//  player.x -= (uint16_t)F_WIDTH;

  screenNo++;
  // generate terrain for field_ptr[1][3]
  terrain_gen.generateScreen(field_ptr[1], field_ptr[3], field_ptr[0], field_ptr[2]);
}


void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);
//  player.x = 110 << FBITS;
//  player.y = 40 << FBITS;
  camera.focus_x = F_WIDTH-5;
}


void loop() {
  if (!(arduboy.nextFrameDEV()))
    return;

  arduboy.pollButtons();

  arduboy.fillScreen(WHITE);
  counter++;

  if (arduboy.justPressed(B_BUTTON)) {
    debug_info_toggle = !debug_info_toggle;
    arduboy.setFrameRate(debug_info_toggle ? 20 : 30);
  }

  // check overscroll
  if (camera.x > (uint16_t)F_WIDTH)
    screenOverlap();

//  player.process();
//  player.processAnim();

  //camera.focus_x = player.x;
  //camera.focus_y = player.y;
  camera.processControls();
  camera.process();

  drawField(camera);
  if (debug_info_toggle)
    camera.drawDebugOverlay();
//  player.draw(camera);

  arduboy.display();
  arduboy.idle();
}
