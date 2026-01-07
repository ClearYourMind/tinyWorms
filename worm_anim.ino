#include "Arduboy2.h"

#include "common.h"
#include "camera.h"

Arduboy2 arduboy;
Sprites sprites;

#include "fixedmath.h"
#include "model.h"
#include "models.h"

uint16_t counter = 0;
bool debug_info_toggle = false;


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

Camera camera;
Model* model_1;
Model* model_2;
Model* model_3;

void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);
  camera.cell_x = 0;
  camera.cell_y = 0;

  model_1 = new Model(64 << FBITS, 32 << FBITS, wormBase, 23);
  model_2 = new Model(64 << FBITS, 32 << FBITS, wormEyePoly, 5);
  model_3 = new Model(68 << FBITS, 32 << FBITS, wormEyePoly, 5);
}


void loop() {
  if (!(arduboy.nextFrameDEV()))
    return;

  arduboy.pollButtons();

  arduboy.fillScreen(0);
  counter++;

  if (arduboy.justPressed(B_BUTTON)) {
    debug_info_toggle = !debug_info_toggle;
    arduboy.setFrameRate(debug_info_toggle ? 20 : 30);
  }

  camera.processControls();
  camera.process();
  
  if (debug_info_toggle) {
    camera.drawDebugOverlay();
  }

  model_1->drawOutline(camera, WHITE);
  model_2->drawFill(camera, WHITE);
  model_3->drawFill(camera, WHITE);

  arduboy.display();

}
