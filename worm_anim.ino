#include "Arduboy2.h"

#include "common.h"

Arduboy2 arduboy;
Sprites sprites;

#include "camera.h"
#include "fixedmath.h"
#include "models.h"
#include "model_tester.h"
#include "player.h"

uint16_t counter = 0;
bool debug_info_toggle = false;


Camera camera;
ModelTester tester;

void debug_stop(int32_t val_1, int32_t val_2, const char message[] = NULL) {
  arduboy.fillScreen(0);
  arduboy.setTextColor(WHITE);
  arduboy.setCursor(32, 0);
  arduboy.print("DEBUG STOP");

  arduboy.setCursor(32, 16);
  arduboy.print(message);
  arduboy.setCursor(32, 40);
  arduboy.print(val_1);
  arduboy.setCursor(80, 40);
  arduboy.print(val_2);
  arduboy.display();
  arduboy.waitNoButtons();
  while (!arduboy.anyPressed(255));

}
void debug_stop(int32_t value, const char message[] = NULL) {
  debug_stop(value, 0, message);
}


void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);

  tester.add_model(test_grid, 64);
  tester.add_model(w_pistol, 8);

  debug_stop(F_WIDTH, F_HEIGHT, "W & H");
  debug_stop(F_WIDTH_H, F_HEIGHT_H, "Half W & H");
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
  if (!tester.control_captured)
    camera.processControls();
  else {
    camera.focus_x = tester.focus_x;
    camera.focus_y = tester.focus_y;
  }
  camera.process();

  tester.draw(camera);

  if (debug_info_toggle)
    camera.drawDebugOverlay();

  arduboy.display();

}
