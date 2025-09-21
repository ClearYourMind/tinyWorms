#include "Arduboy2.h"

Arduboy2 arduboy;
Sprites sprites;

//#include "FixedMath.c"   // not using fmath by this time

#define MAX_LINES 16

uint16_t counter = 0;

uint8_t const ground[] PROGMEM = {
  7, 8, 
  0x1C, 0x3E, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C
};
uint32_t field[MAX_LINES] = {
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,

  0x000C0000,
  0x003F0000,
  0x007FC000,
  0x00FFE000,

  0x01FFF000,
  0x03FFFC00,
  0x07F3FE00,
  0x0FC0FF03,

  0x1F003FFF,
  0xFC000FFF,
  0xF80000FF,
  0xE0000000
};

class Player {
  public:
    bool landed;
    int8_t dir; // -1 .. 1

    int32_t x;
    int32_t y;
    int32_t dx;
    int32_t dy;
    int32_t walk_speed;
    int8_t* cx;
    int8_t* cy;
    Player();
    ~Player();
    void draw();
    void process();
};

Player::Player() {
  walk_speed = 1;
  cx = new int8_t[3];
  cy = new int8_t[3];
};

Player::~Player() {
  delete [] cx;
  delete [] cy;
};

void Player::draw() {
  int8_t osc = counter % 2;
  arduboy.drawFastVLine(x-1, y, 16, osc);
  arduboy.drawFastVLine(x, y, 16, osc);
  arduboy.drawFastVLine(x+1, y, 16, osc);

  arduboy.drawRect(cx[0] << 2, cy[0] << 2, 7, 7, osc);
  arduboy.drawRect(cx[1] << 2, cy[1] << 2, 7, 7, osc);
};

void Player::process() {
  int32_t cell;
  cx[0] = (x - 2) >> 2;
  cy[0] = (y + 16) >> 2;
  cx[1] = (x + 2) >> 2;
  cy[1] = (y + 16) >> 2;

  if (arduboy.pressed(UP_BUTTON)) {
    y = max(0, y - walk_speed);
  };
  if (arduboy.pressed(DOWN_BUTTON)) {
    y = min(HEIGHT, y + walk_speed);
  };
  if (arduboy.pressed(LEFT_BUTTON)) {
    x = max(0, x - walk_speed);
  };
  if (arduboy.pressed(RIGHT_BUTTON)) {
    x = min(WIDTH, x + walk_speed);
  };

  if (arduboy.pressed(A_BUTTON)) {
    field[cy[0]] = field[cy[0]] | (0x80000000 >> cx[0]);
  };
  cell = (field[cy[0]] & (0x80000000 >> cx[0])) > 0 ? 1 : 0;
  arduboy.setCursor(0, 0);
  arduboy.print(cell);
  cell = (field[cy[1]] & (0x80000000 >> cx[1])) > 0 ? 1 : 0;
  arduboy.setCursor(8, 0);
  arduboy.print(cell);
};

Player player;

void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);
  player.x = 5;
  player.y = 5;
}

uint8_t yy = 0;
uint8_t xx = 0;
uint8_t line_count = 1;

void loop() {
  if (!(arduboy.nextFrameDEV()))
    return;

  arduboy.pollButtons();

  arduboy.fillScreen(WHITE);
  counter++;

  for (uint8_t j=0; j<MAX_LINES; j++) {
    yy = j << 2;
    for (uint8_t i=0; i<32; i++)
    if ((field[j] & (0x80000000 >> i)) > 0) {
      xx = i << 2;
      sprites.drawErase(xx, yy, ground, 0);
    };
  };

  player.process();
  if (counter % 2 == 0)
    player.draw();

  arduboy.display();

}
