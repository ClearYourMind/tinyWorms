#include "Arduino.h"
#include "terrain_generator.h"
#include "fixedmath.h"

TerrainGenerator::TerrainGenerator(uint8_t initial_seed = 0) {
    seed = initial_seed;
    //randomSeed(seed);
   camera = Camera();
   camera.x = (uint16_t)F_WIDTH;
   camera.cell_x = CELL_COUNT_X;
}


void TerrainGenerator::generateScreen(
        uint32_t screen_hi[CELL_COUNT_Y],
        uint32_t screen_lo[CELL_COUNT_Y],
        uint32_t prev_screen_hi[CELL_COUNT_Y],
        uint32_t prev_screen_lo[CELL_COUNT_Y]
    ) {
    screen[0] = screen_hi;
    screen[1] = screen_lo;
    // get last column and clear screen
    for (uint8_t y = 0; y < CELL_COUNT_Y; y++) {
        screen_hi[y] = (prev_screen_hi[y] & 1) << (CELL_COUNT_X-1);
        screen_lo[y] = (prev_screen_lo[y] & 1) << (CELL_COUNT_X-1);
    }
    uint8_t height = 0;
    uint8_t cell_y;         // move screen index calc code into getCell/setCell
    uint8_t f_screen; // 0..1

    for (uint8_t y = 0; y < CELL_COUNT_Y * 2; y++) {
        f_screen = y >> 4;    // y / CELL_COUNT_Y // move screen index calc code into getCell/setCell
        cell_y = y % CELL_COUNT_Y;
        if (getCell(screen[f_screen], 0, cell_y))
            height++;
        else if (height > 0) {
            generateLine(0, y, height, CELL_COUNT_X >> 1);
            height = 0;
        }
    }
    if (height > 0)
        generateLine(0, CELL_COUNT_Y * 2 - 1, height, CELL_COUNT_X >> 1);
}


void TerrainGenerator::generateLine(uint8_t start_x, uint8_t start_y, uint8_t start_height, int8_t max_length) {
    if (max_length < 1 || start_x > CELL_COUNT_X) {
        // arduboy.fillScreen();
        // arduboy.setCursor(0, 0);  arduboy.print("line ended");
        // arduboy.display();
        // arduboy.waitNoButtons();
        // while (!arduboy.anyPressed(255));
        return;
    }

    int8_t up_down = (random(1, CELL_COUNT_Y*2) <= start_y) ? -1: 1;
    int8_t thick_thin = (random(3, CELL_COUNT_Y) <= start_height) ? -1: 1;
    int16_t d_height = random(1 << (FBITS-4), 1 << (FBITS-2));
    int16_t f_height_offset = 0;    // cumulative height change along line
    int16_t line_y = start_y << FBITS;
    int8_t start_line_y;
    int8_t end_line_y;
    uint8_t length = random(1, max_length);
    uint8_t cell_y;         // move screen index calc code into getCell/setCell
    uint8_t f_screen; // 0..1

    for (uint8_t x = start_x; x < start_x + length + 1; x++) {
        f_height_offset += d_height * thick_thin;
        line_y = line_y + (up_down << (FBITS-1)); // +0.5 .. -0.5
        start_line_y = (line_y >> FBITS) - (start_height + (f_height_offset >> FBITS));
        start_line_y = min(max(start_line_y, 0), CELL_COUNT_Y * 2);
        end_line_y = (line_y >> FBITS) - (f_height_offset >> FBITS);
        end_line_y = min(max(end_line_y, 0), CELL_COUNT_Y * 2);
        if ((end_line_y - start_line_y) < 2) {
           end_line_y++;
           break;
        }
        for (uint8_t y = start_line_y; y < end_line_y; y++) {
            f_screen = (y >> 4) % 2;    // y / CELL_COUNT_Y // move screen index calc code into getCell/setCell
            cell_y = y % CELL_COUNT_Y;
            setCell(screen[f_screen], x, cell_y, true);
        }
        arduboy.fillScreen();
        drawField(camera);
        arduboy.setCursor(0, 0);  arduboy.print(start_x);
        arduboy.setCursor(24, 0); arduboy.print(start_y);
        arduboy.setCursor(48, 0); arduboy.print(start_height);
        arduboy.setCursor(72, 0); arduboy.print(max_length);
        arduboy.setCursor(24, 8); arduboy.print(start_line_y);
        arduboy.setCursor(48, 8); arduboy.print(end_line_y);
        arduboy.setCursor(72, 8); arduboy.print(length);
        arduboy.setCursor(0, 16); arduboy.print(up_down);
        arduboy.setCursor(24,16); arduboy.print(thick_thin);

        arduboy.display();
        // arduboy.waitNoButtons();
        // while (!arduboy.anyPressed(255));
    }
    // arduboy.fillScreen();
    // arduboy.setCursor(0, 0);  arduboy.print(start_x);
    // arduboy.setCursor(24, 0); arduboy.print(length);
    // arduboy.setCursor(48, 0); arduboy.print(start_x + length);
    // arduboy.setCursor(48, 8); arduboy.print(CELL_COUNT_X - (start_x + length));
    // arduboy.display();
    // arduboy.waitNoButtons();
    // while (!arduboy.anyPressed(255));
    generateLine(start_x + length, end_line_y, end_line_y-start_line_y, (CELL_COUNT_X - (start_x + length)) >> 1);
}

