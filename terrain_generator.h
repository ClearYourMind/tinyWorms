#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include "Arduino.h"
#include "common.h"
#include "camera.h"

extern void drawField(Camera camera);
extern void setCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y, bool value);
extern bool getCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y);

class TerrainGenerator {
  public:
    uint8_t seed;
    uint32_t *screen[2];
    Camera camera;

    TerrainGenerator(uint8_t initial_seed = 0);

    void generateLine(uint8_t start_x, uint8_t start_y, uint8_t start_height, int8_t max_length);
    void generateScreen(
      uint32_t screen_hi[CELL_COUNT_Y],
      uint32_t screen_lo[CELL_COUNT_Y],
      uint32_t prev_screen_hi[CELL_COUNT_Y],
      uint32_t prev_screen_lo[CELL_COUNT_Y]
    );
};

#endif
