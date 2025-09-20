#include "Arduboy2.h"

Arduboy2 arduboy;
Sprites sprites;

#define MAX_LINES 24

uint8_t const ground[] PROGMEM = {
  7, 8, 
  0x1C, 0x3E, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C
};
uint32_t field[MAX_LINES] = {
  0xFFFFFFFF,
  0x00FFFFFF,
  0x0001FF1F,
  0xE0003003,

  0xC0000001,
  0x00C00007,
  0x03F0003F,
  0x01E0380F,

  0x0001FE07,
  0x0007FF06,
  0xC00FFF0D,
  0xF00FFE0E,

  0x3F03E01A,
  0xCF80001D,
  0x31C00035,
  0x0EE0007B
};

void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);
  //arduboy.initRandomSeed();
}

uint16_t counter = 0;
uint8_t yy = 0;
uint8_t xx = 0;
uint8_t line_count = 1;

void loop() {
  if (!(arduboy.nextFrameDEV()))
    return;

  arduboy.pollButtons();

  arduboy.fillScreen(WHITE);
  // counter++;
  // if ((counter % MAX_LINES) == 0)
  //   line_count = (line_count + 1) % MAX_LINES;

  for (uint8_t j=0; j<MAX_LINES; j++) {
    yy = j << 2;
    for (uint8_t i=0; i<32; i++)
    if ((field[j] >> i & 1) == 1) {
      xx = (30-i) << 2;
      sprites.drawErase(xx, yy, ground, 0);
    };
  };

  arduboy.display();

//  test_animation();
}
