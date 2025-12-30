#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include "Arduino.h"
#include "common.h"

class TerrainGenerator {
  private:
    uint8_t seed;
    uint8_t last_height[CELL_COUNT_X]; // Высота поверхности в предыдущем столбце
    
    // Параметры генерации
    int8_t min_height = 4;  // Минимальная высота (от верха)
    int8_t max_height = 12; // Максимальная высота
    int8_t max_step = 1;    // Максимальный шаг изменения высоты между столбцами
    
    // Вспомогательные функции
    void setCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y, bool value);
    bool getCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y);
    void smoothTerrain(uint32_t field[CELL_COUNT_Y]);
    
  public:
    TerrainGenerator(uint8_t initial_seed = 0);
    
    void generateNextScreen(uint32_t new_screen[CELL_COUNT_Y], uint32_t previous_screen[CELL_COUNT_Y]);
    void generateScreen(uint32_t screen[CELL_COUNT_Y]);
    void setSeed(uint8_t new_seed);
};

#endif