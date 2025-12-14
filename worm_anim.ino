#include "Arduboy2.h"

#include "animations.h"
#include "common.h"

Arduboy2 arduboy;
Sprites sprites;

//#include "player.h"
#include "camera.h"
#include "fixedmath.h"

uint16_t counter = 0;
bool debug_info_toggle = false;

uint8_t const ground[] PROGMEM = {
  7, 8, 
  0x1C, 0x3E, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C
};

uint32_t field[4][CELL_COUNT_Y] = {
  {
    0xFFFFFFE0,
    0x0000FFC0,
    0x00003F80,
    0x00000601,

    0x000C0001,
    0x003F0003,
    0x007FE07F,
    0x00FFE007,

    0x01FFF001,
    0x07FFFC03,
    0x07F3FE03,
    0x8FC0FF03,

    0x9F003FFF,
    0xFC000FFF,
    0xF80000FF,
    0xE0000000
  },
  {
    ~0xFFFFFFE0,
    ~0x0000FFC0,
    ~0x00003F80,
    ~0x00000601,

    ~0x000C0001,
    ~0x003F0003,
    ~0x007FE07F,
    ~0x00FFE007,

    ~0x01FFF001,
    ~0x07FFFC03,
    ~0x07F3FE03,
    ~0x8FC0FF03,

    ~0x9F003FFF,
    ~0xFC000FFF,
    ~0xF80000FF,
    ~0xE0000000
  },
  {
    ~0xFFFFFFE0,
    ~0x0000FFC0,
    ~0x00003F80,
    ~0x00000601,

    ~0x000C0001,
    ~0x003F0003,
    ~0x007FE07F,
    ~0x00FFE007,

    ~0x01FFF001,
    ~0x07FFFC03,
    ~0x07F3FE03,
    ~0x8FC0FF03,

    ~0x9F003FFF,
    ~0xFC000FFF,
    ~0xF80000FF,
    ~0xE0000000
  },
  {
    0xFFFFFFE0,
    0x0000FFC0,
    0x00003F80,
    0x00000601,

    0x000C0001,
    0x003F0003,
    0x007FE07F,
    0x00FFE007,

    0x01FFF001,
    0x07FFFC03,
    0x07F3FE03,
    0x8FC0FF03,

    0x9F003FFF,
    0xFC000FFF,
    0xF80000FF,
    0xE0000000
  }

};

Camera camera;


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


void drawFrame(int8_t x, int8_t y, uint8_t frame_id) {
  uint8_t count = pgm_read_byte_near(frame_list[frame_id] + 0);
  struct {
    uint8_t id;
    int8_t offset_x;
    int8_t offset_y;
  } tile;
  for (uint8_t i=0; i<count; i++) {
    tile.id = pgm_read_byte_near(frame_list[frame_id] + i*3 + 1);
    tile.offset_x = pgm_read_byte_near(frame_list[frame_id] + i*3 + 2);
    tile.offset_y = pgm_read_byte_near(frame_list[frame_id] + i*3 + 3);
    sprites.drawExternalMask(
      x + tile.offset_x,
      y + tile.offset_y,
      spritesheet_worm, spritesheet_worm_mask,
      tile.id, tile.id
    ); 
  };
}

void drawField() {
  uint8_t yy = 0;
  uint8_t xx = 0;

  for (uint8_t j=camera.cell_y; j<CELL_COUNT_Y; j++) {
    yy = (j << 2) - (camera.y >> FBITS);
    for (uint8_t i=camera.cell_x; i<CELL_COUNT_X; i++) {
      xx = (i << 2) - (camera.x >> FBITS);
      if (field[0][j] & (0x80000000 >> i))
        sprites.drawErase(xx, yy, ground, 0);
    };
    for (uint8_t i=0; i<camera.cell_x; i++) {
      xx = ((i + CELL_COUNT_X) << 2) - (camera.x >> FBITS);
      if (field[1][j] & (0x80000000 >> i))
        sprites.drawErase(xx, yy, ground, 0);
    };
  };

  for (uint8_t j=0; j<camera.cell_y; j++) {
    yy = ((j + CELL_COUNT_Y) << 2) - (camera.y >> FBITS);
    for (uint8_t i=camera.cell_x; i<CELL_COUNT_X; i++) {
      xx = (i << 2) - (camera.x >> FBITS);
      if (field[2][j] & (0x80000000 >> i))
        sprites.drawErase(xx, yy, ground, 0);
    };
    for (uint8_t i=0; i<camera.cell_x; i++) {
      xx = ((i + CELL_COUNT_X) << 2) - (camera.x >> FBITS);
      if (field[3][j] & (0x80000000 >> i))
        sprites.drawErase(xx, yy, ground, 0);
    };
  };
}


//Player player;

void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);
//  player.x = 110 << FBITS;
//  player.y = 40 << FBITS;
}

void loop() {
  if (!(arduboy.nextFrameDEV()))
    return;

  arduboy.pollButtons();

  arduboy.fillScreen(WHITE);
  counter++;

  //player.process();
  camera.processControls();
  camera.process();
  
  drawField();
  camera.drawDebugOverlay();
  //player.processAnim();
  //player.draw();

  arduboy.display();

}
