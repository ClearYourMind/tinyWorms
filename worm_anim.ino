#include "common.h"
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

  counter++;
  arduboy.pollButtons();

  test_animation();
}
