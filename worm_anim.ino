#include "Arduboy2.h"

Arduboy2 arduboy;
Sprites sprites;

#include "test_animation.h"

void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(20);
}


void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.pollButtons();

  test_animation();
}
