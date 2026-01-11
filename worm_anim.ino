#include "Arduboy2.h"

#include "common.h"

Arduboy2 arduboy;
Sprites sprites;

#include "camera.h"
#include "fixedmath.h"
#include "models.h"
#include "model_tester.h"

uint16_t counter = 0;
bool debug_info_toggle = false;


Camera camera;
ModelTester tester;

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



void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);

  tester.add_model(wormBase, 23);
  tester.add_model(wormBase, 23);
}


void loop() {
  if (!(arduboy.nextFrameDEV()))
    return;

  arduboy.pollButtons();

  counter++;

  if (!tester.control_captured)
  if (arduboy.justPressed(B_BUTTON)) {
    debug_info_toggle = !debug_info_toggle;
    arduboy.setFrameRate(debug_info_toggle ? 20 : 30);
  }

  tester.process();
  camera.focus_x = tester.focus_x;
  camera.focus_y = tester.focus_y;

  if (!tester.control_captured)
    camera.process();

  tester.draw(camera);

  if (debug_info_toggle)
    camera.drawDebugOverlay();

  arduboy.display();

}
