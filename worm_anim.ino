#include "Arduboy2.h"
#include "FixedMath.c"
#include "spritesheet.h"
#include "frames.c"
#include "avr/pgmspace.h"

Arduboy2 arduboy;
Sprites sprites;

typedef struct {
  uint8_t tile_id;
  int8_t offset_x;
  int8_t offset_y;
} tile_data;

#define MAX_ANIMATIONS 6
// frame_count, frame ids
uint8_t const anim_stand_r[] PROGMEM = {1, 0};
uint8_t const anim_walk_r[] PROGMEM = {5, 1, 1, 2, 2, 2};
uint8_t const anim_stand_r_u[] PROGMEM = {1, 3};
uint8_t const anim_walk_r_u[] PROGMEM = {5, 4, 4, 5, 5, 5};
uint8_t const anim_stand_r_d[] PROGMEM = {1, 6};
uint8_t const anim_walk_r_d[] PROGMEM = {5, 7, 7, 8, 8, 8};

uint8_t* const anim_list[MAX_ANIMATIONS] = {
  anim_stand_r,
  anim_walk_r,
  anim_stand_r_u,
  anim_walk_r_u,
  anim_stand_r_d,
  anim_walk_r_d
};

typedef struct {
  uint8_t anim_id;
  uint8_t anim_frame;
  uint8_t anim_count;
  uint8_t* anim_frames;
} anim_data;

anim_data anim = {
  0, 0, 1, anim_stand_r
};

void drawFrame(int8_t x, int8_t y, uint8_t frame_id) {
  uint8_t count = pgm_read_byte_near(frame_list[frame_id] + 0);
  tile_data tile;
  for (uint8_t i=0; i<count; i++) {
    tile.tile_id = pgm_read_byte_near(frame_list[frame_id] + i*3 + 1);
    tile.offset_x = pgm_read_byte_near(frame_list[frame_id] + i*3 + 2);
    tile.offset_y = pgm_read_byte_near(frame_list[frame_id] + i*3 + 3);
    sprites.drawExternalMask(
      x + tile.offset_x,
      y + tile.offset_y,
      spritesheet_worm, spritesheet_worm_mask,
      tile.tile_id, tile.tile_id
    ); 
  };
}


void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);
}

int fillColor = WHITE;
uint16_t counter = 0;
uint8_t frame;

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  counter++;
  arduboy.pollButtons();


  arduboy.fillScreen(fillColor);

  // change background
  if (arduboy.justPressed(A_BUTTON))
    fillColor = (fillColor + 1) % 2;

  if (arduboy.justPressed(RIGHT_BUTTON)) {
    anim.anim_id = (anim.anim_id + 1) % MAX_ANIMATIONS;
    anim.anim_frame = 0;
    anim.anim_count = pgm_read_byte_near(anim_list[anim.anim_id]);
  };
  if (arduboy.justPressed(LEFT_BUTTON)) {
    anim.anim_id = max(1, anim.anim_id) - 1;
    anim.anim_frame = 0;
    anim.anim_count = pgm_read_byte_near(anim_list[anim.anim_id]);
  };

  frame = pgm_read_byte_near(anim_list[anim.anim_id] + anim.anim_frame + 1);
  drawFrame(64, 32, frame);
  anim.anim_frame = (anim.anim_frame + 1) % anim.anim_count;

  arduboy.setCursor(0, 0);
  arduboy.print(anim.anim_id, HEX);
  arduboy.setCursor(0, 8);
  arduboy.print(anim.anim_frame);
  arduboy.setCursor(0, 16);
  arduboy.print(frame);

  arduboy.display();
}
