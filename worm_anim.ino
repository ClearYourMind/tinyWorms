#include "Arduboy2.h"


Arduboy2 arduboy;
Sprites sprites;

//#include "FixedMath.c"   // not using fmath by this time

#define MAX_LINES 16
#define CELL_COUNT 7      // cells for checking collision
#define SIGN(x) ((x) > 0) - ((x) < 0)

uint16_t counter = 0;

uint8_t const ground[] PROGMEM = {
  7, 8, 
  0x1C, 0x3E, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C
};
uint32_t field[MAX_LINES] = {
  0xFFFFFFE0,
  0x0000FFC0,
  0x00003F80,
  0x00000601,

  0x000C0001,
  0x003F0003,
  0x007FE07F,
  0x00FFE007,

  0x01FFF001,
  0x07FFFC03,
  0x07F3FE03,
  0x8FC0FF03,

  0x9F003FFF,
  0xFC000FFF,
  0xF80000FF,
  0xE0000000
};

class Player {
  public:
    bool landed;
    bool o_landed;
    int8_t dir; // -1 .. 1

    int32_t x;
    int32_t y;
    int32_t dx;
    int32_t dy;
    int32_t walk_speed;
    int8_t* cx;
    int8_t* cy;
    int8_t cells;  // binary set of flags
    Player();
    ~Player();
    void draw();
    void process();
    void checkCells();
};

Player::Player() {
  walk_speed = 1;
  cx = new int8_t[CELL_COUNT];
  cy = new int8_t[CELL_COUNT];
};

Player::~Player() {
  delete [] cx;
  delete [] cy;
};

void Player::draw() {
  uint8_t osc = counter % 2;
  arduboy.drawFastVLine(x-1, y, 16, osc);
  arduboy.drawFastVLine(x, y, 16, osc);
  arduboy.drawFastVLine(x+1, y, 16, osc);

  if (arduboy.pressed(B_BUTTON))
  for (uint8_t c = 0; c < CELL_COUNT; c++)
    arduboy.drawRect(cx[c] << 2, cy[c] << 2, 7, 7, osc);

  arduboy.setCursor(0, 24);
  arduboy.print(cells & 1);
  arduboy.setCursor(8, 24);
  arduboy.print((cells >> 1) & 1);
  arduboy.setCursor(0, 16);
  arduboy.print((cells >> 2) & 1);
  arduboy.setCursor(8, 16);
  arduboy.print((cells >> 3) & 1);
  arduboy.setCursor(0, 8);
  arduboy.print((cells >> 4) & 1);
  arduboy.setCursor(8, 8);
  arduboy.print((cells >> 5) & 1);
  arduboy.setCursor(4, 0);
  arduboy.print((cells >> 6) & 1);

  if (landed) {
    arduboy.setCursor(16, 0);
    arduboy.print("L");
  };
  arduboy.setCursor(16, 8);
  arduboy.print(dx);
  arduboy.setCursor(24, 8);
  arduboy.print(dy);

};

void Player::checkCells() {
  //update cells
  //  feet on ground
  cx[0] = (x - 2) >> 2;
  cy[0] = (y + 16) >> 2;
  cx[1] = (x + 2) >> 2;
  cy[1] = (y + 16) >> 2;
  //  climbing cells
  cx[2] = (x - 2) >> 2;
  cy[2] = (y + 12) >> 2;
  cx[3] = (x + 2) >> 2;
  cy[3] = (y + 12) >> 2;
  //  wall hit (stopping) cells
  cx[4] = (x - 2) >> 2;
  cy[4] = (y + 8) >> 2;
  cx[5] = (x + 2) >> 2;
  cy[5] = (y + 8) >> 2;
  //  head
  cx[6] = x >> 2;
  cy[6] = (y + 4) >> 2;

  int32_t cell;
  cells = 0;
  for (int8_t c=0; c<CELL_COUNT; c++) {
    cell = (field[cy[c]] & (0x80000000 >> cx[c])) > 0 ? 1 : 0;
    cells |= (cell << c);
  }

}

void Player::process() {

  if (landed) {
    if (arduboy.pressed(LEFT_BUTTON)) {
      dir = -1;
      if ((cells & 0x10) == 0) {  // stop cell is free
        x = max(0, x - walk_speed);
        if ((cells & 0x0F) == 0x07)
          y -= walk_speed;
        if (((cells & 0x0F) == 0x02) | ((cells & 0x0F) == 0x0B))
          y += walk_speed;
      }
    };
    if (arduboy.pressed(RIGHT_BUTTON)) {
      dir = 1;
      if ((cells & 0x20) == 0) {  // stop cell is free
        x = min(WIDTH, x + walk_speed);
        if ((cells & 0x0F) == 0x0B)
          y -= walk_speed;
        if (((cells & 0x0F) == 0x01) | ((cells & 0x0F) == 0x07))
          y += walk_speed;
      };
    };
    if (arduboy.pressed(A_BUTTON)) {
      bool jump = false;
      if ((cells & 0x40) == 0) { // head cell is free
        if ((dir > 0) & ((cells & 0x20) == 0)) {   // facing right
          jump = true;
        } else 
        if ((dir < 0) & ((cells & 0x10) == 0)) {   // facing left
          jump = true;
        };
      };
      if (jump) {
        dy = -4;
        dx = 2 * dir;
      };

    };
  };

  y += dy;
  x += dx;

  checkCells(); // before correcting player position after collision

  dy = min(dy + 1, 4); // gravity
  landed = (cells & 0x03) > 0;
  o_landed = landed;

  // process speeds
  if (landed) {
    if (dy > 0) {
      dy = 0;
      dx = 0;
    };
  } else {
    if (dy < 0) {
      if ((cells & 0x40) > 0) {   // head hit
        dy = 0;
        y++;
      };
    };
    if (dx != 0) {
      if (((cells & 0x28) > 0) | ((cells & 0x14) > 0)) {
        x -= SIGN(dx);
        dx = -dx;
      };
    };
  }

  // unstuck
  if ((cells & 0x0C) == 0x0C) // both climbing cells in ground
    y--;

  checkCells(); // after corrections and before player moving

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
  player.draw();

  arduboy.display();

}
