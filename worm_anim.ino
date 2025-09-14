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

#define MAX_ANIMATIONS 26
// frame_count, frame ids
uint8_t const anim_stand_r[] PROGMEM =    {1, 0x00};
uint8_t const anim_walk_r[] PROGMEM =     {4, 0x01, 0x01, 0x02, 0x02};
uint8_t const anim_stand_r_u[] PROGMEM =  {1, 0x03};
uint8_t const anim_walk_r_u[] PROGMEM =   {4, 0x04, 0x04, 0x05, 0x05};
uint8_t const anim_stand_r_d[] PROGMEM =  {1, 0x06};
uint8_t const anim_walk_r_d[] PROGMEM =   {4, 0x07, 0x07, 0x08, 0x08};
uint8_t const anim_jumpstart_r[] PROGMEM ={2, 0x09, 0x0A};
uint8_t const anim_jump_r[] PROGMEM =     {2, 0x0B, 0x0C};
uint8_t const anim_jumpland_r[] PROGMEM = {3, 0x0D, 0x0E, 0x0F};
uint8_t const anim_slide_r[] PROGMEM =    {2, 0x10, 0x11};
uint8_t const anim_slide_r_u[] PROGMEM =  {2, 0x12, 0x13};
uint8_t const anim_slide_r_d[] PROGMEM =  {2, 0x14, 0x15};
uint8_t const anim_recover_r[] PROGMEM =  {5, 0x16, 0x17, 0x18, 0x19, 0x1A};

uint8_t const anim_stand_l[] PROGMEM =    {1, 0x1B};
uint8_t const anim_walk_l[] PROGMEM =     {4, 0x1C, 0x1C, 0x1D, 0x1D};
uint8_t const anim_stand_l_u[] PROGMEM =  {1, 0x1E};
uint8_t const anim_walk_l_u[] PROGMEM =   {4, 0x1F, 0x1F, 0x20, 0x20};
uint8_t const anim_stand_l_d[] PROGMEM =  {1, 0x21};
uint8_t const anim_walk_l_d[] PROGMEM =   {4, 0x22, 0x22, 0x23, 0x23};
uint8_t const anim_jumpstart_l[] PROGMEM ={2, 0x24, 0x25};
uint8_t const anim_jump_l[] PROGMEM =     {2, 0x26, 0x27};
uint8_t const anim_jumpland_l[] PROGMEM = {3, 0x28, 0x29, 0x2A};
uint8_t const anim_slide_l[] PROGMEM =    {2, 0x2B, 0x2C};
uint8_t const anim_slide_l_u[] PROGMEM =  {2, 0x2D, 0x2E};
uint8_t const anim_slide_l_d[] PROGMEM =  {2, 0x2F, 0x30};
uint8_t const anim_recover_l[] PROGMEM =  {5, 0x31, 0x32, 0x33, 0x34, 0x35};

uint8_t* const anim_list[MAX_ANIMATIONS] = {
  anim_stand_r,
  anim_walk_r,
  anim_stand_r_u,
  anim_walk_r_u,
  anim_stand_r_d,
  anim_walk_r_d,
  anim_jumpstart_r,
  anim_jump_r,
  anim_jumpland_r,
  anim_slide_r,
  anim_slide_r_u,
  anim_slide_r_d,
  anim_recover_r,

  anim_stand_l,
  anim_walk_l,
  anim_stand_l_u,
  anim_walk_l_u,
  anim_stand_l_d,
  anim_walk_l_d,
  anim_jumpstart_l,
  anim_jump_l,
  anim_jumpland_l,
  anim_slide_l,
  anim_slide_l_u,
  anim_slide_l_d,
  anim_recover_l
};

typedef struct {
  uint8_t anim_id;
  uint8_t anim_frame;
  uint8_t anim_count;
} anim_data;

anim_data anim = {
  0, 0, 1
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
  arduboy.setFrameRate(20);
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

  // switch animation
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

  arduboy.setCursor(0, 0);
  arduboy.print(anim.anim_id, HEX);
  arduboy.setCursor(0, 8);
  arduboy.print(anim.anim_frame);
  arduboy.setCursor(0, 16);
  arduboy.print(frame, HEX);

  anim.anim_frame = (anim.anim_frame + counter % 2) % anim.anim_count;
  arduboy.display();
}
