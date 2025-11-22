#include "Arduboy2.h"

Arduboy2 arduboy;
Sprites sprites;

#include "FixedMath.c"
#include "test_animation.h"

#define F_WIDTH (WIDTH << FBITS)
#define MAX_LINES 16
#define CELL_COUNT 7      // cells for checking collision
#define SIGN(x) ((x) > 0) - ((x) < 0)   // evaluates X twice !!!
#define GRAVITY (1 << (FBITS-2))
#define MAX_SPEED (4 << FBITS)

#define AF_DIAG_DOWN  1   // :.
#define AF_DIAG_UP    2   // .:
#define AF_RIGHT      4
#define AF_FALL       8
#define AF_WALK       16
#define AF_SLIDE      32

#define CF_LEFT       1
#define CF_RIGHT      2
#define CF_UP         4
#define CF_DOWN       8
#define CF_JUMP       16
#define CF_SHOOT      32

// basic animations to switch to
#define AN_STAND  1
#define AN_WALK   2
#define AN_JUMP   3
#define AN_FALL   4
#define AN_LAND   5

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


uint8_t setFlagAsBool(uint8_t flags, uint8_t flag, bool bool_value) {
  return bool_value ? (flags | flag) : (flags & ~((uint8_t)flag));
}

class Player {
  public:
    bool landed;
    bool o_landed;
    bool can_move;     // disable controls during some animations (land, recover, jump, fall, hurt..)
    bool want_jump;    // use when canMove=false but jump button pressed to jump anyway later
    bool want_shoot;   // use when canMove=false but shoot button pressed to shoot anyway later
    int8_t dir; // -1 .. 1

    int32_t x;
    int32_t y;
    int32_t dx;
    int32_t dy;
    int32_t walk_speed;
    int32_t jump_speed_y;
    int32_t jump_speed_x;
    int8_t* cx;
    int8_t* cy;
    uint8_t cells;  // binary set of flags
    uint8_t anim_flags;
    uint8_t command_flags;
    uint8_t frame;
    uint8_t frame_count;
    uint8_t anim_action;
    bool anim_ended;
    int8_t* anim; // array

    Player();
    ~Player();
    void draw();
    void process();
    void processControls();
    void processAnim();
    void switchAnim(uint8_t _anim_action);
    void checkCells();
    void commandFromKeys();
};

Player::Player() {
  walk_speed = 1 << FBITS;
  jump_speed_y = 3 << FBITS;
  jump_speed_x = 3 << (FBITS-1);  // 1.5
  cx = new int8_t[CELL_COUNT];
  cy = new int8_t[CELL_COUNT];
  dir = 1;
  can_move = true;
  want_jump = false;
};

Player::~Player() {
  delete [] cx;
  delete [] cy;
};

void Player::draw() {
  uint8_t osc = counter % 2;
  uint32_t _x = x >> FBITS;
  uint32_t _y = y >> FBITS;

  if (anim == NULL) {
    arduboy.drawFastVLine(_x-1, _y, 16, osc);
    arduboy.drawFastVLine(_x, _y, 16, osc);
    arduboy.drawFastVLine(_x+1, _y, 16, osc);
  } else {
    // draw anim frame
    anim_ended = (frame + osc == frame_count);
    frame = (frame + osc) % frame_count;    // osc = 0..1. Frame added every 2nd step
    uint8_t _frame = pgm_read_byte_near(anim + frame + 1);
    drawFrame(_x, _y + 8, _frame);
  }

  if (arduboy.pressed(B_BUTTON)) {
    for (uint8_t c = 0; c < CELL_COUNT; c++)
      arduboy.drawRect(cx[c] << 2, cy[c] << 2, 7, 7, osc);
  };

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
  arduboy.setCursor(24, 0);
  arduboy.print(x);
  arduboy.setCursor(24, 8);
  arduboy.print(y);

  arduboy.setCursor(60, 0);
  arduboy.print(dx);
  arduboy.setCursor(60, 8);
  arduboy.print(dy);

  // draw flags
  arduboy.setCursor(85, 0);
  arduboy.print(command_flags, HEX);
  arduboy.setCursor(85, 8);
  arduboy.print(anim_flags, HEX);
  
  arduboy.setCursor(96, 0);
  arduboy.print((uint8_t)anim, HEX);
  arduboy.setCursor(96, 8);
  arduboy.print(anim_ended ? "E" : "");
};

void Player::checkCells() {
  //update cells
  uint32_t _x = x >> FBITS;
  uint32_t _y = y >> FBITS;

  //  feet on ground
  cx[0] = (_x - 2) >> 2;
  cy[0] = (_y + 16) >> 2;
  cx[1] = (_x + 2) >> 2;
  cy[1] = (_y + 16) >> 2;
  //  climbing cells
  cx[2] = (_x - 2) >> 2;
  cy[2] = (_y + 12) >> 2;
  cx[3] = (_x + 2) >> 2;
  cy[3] = (_y + 12) >> 2;
  //  wall hit (stopping) cells
  cx[4] = (_x - 2) >> 2;
  cy[4] = (_y + 8) >> 2;
  cx[5] = (_x + 2) >> 2;
  cy[5] = (_y + 8) >> 2;
  //  head
  cx[6] = _x >> 2;
  cy[6] = (_y + 4) >> 2;

  int32_t cell;
  cells = 0;
  for (int8_t c=0; c<CELL_COUNT; c++) {
    cell = (field[cy[c]] & (0x80000000 >> cx[c])) > 0 ? 1 : 0;
    cells |= (cell << c);
  }
}


void Player::commandFromKeys() {
  command_flags = setFlagAsBool(command_flags, CF_LEFT,  arduboy.pressed(LEFT_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_RIGHT, arduboy.pressed(RIGHT_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_UP,    arduboy.pressed(UP_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_DOWN,  arduboy.pressed(DOWN_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_JUMP,  arduboy.pressed(A_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_SHOOT, arduboy.pressed(B_BUTTON));
}


void Player::switchAnim(uint8_t _anim_action) {
  anim_flags = setFlagAsBool(anim_flags, AF_DIAG_DOWN, ((cells & 0x0F) == 0x01) | ((cells & 0x0F) == 0x05) | ((cells & 0x0F) == 0x07));
  anim_flags = setFlagAsBool(anim_flags, AF_DIAG_UP,   ((cells & 0x0F) == 0x02) | ((cells & 0x0F) == 0x0A) | ((cells & 0x0F) == 0x0B));
  anim_flags = setFlagAsBool(anim_flags, AF_RIGHT,     dir == 1);

  anim_action = _anim_action;
  
  // these switch statements can be replaced with 2D array
  switch (_anim_action) {
    case AN_STAND:
      switch (anim_flags) {
        case B00000100:
          anim = anim_stand_r;
          break;
        case B00000000:
          anim = anim_stand_l;
          break;
        case B00000101:
          anim = anim_stand_r_d;
          break;
        case B00000110:
          anim = anim_stand_r_u;
          break;
        case B00000001:
          anim = anim_stand_l_u;
          break;
        case B00000010:
          anim = anim_stand_l_d;
          break;
      }
      break;
    case AN_WALK:
      switch (anim_flags) {
        case B00000000:
          anim = anim_walk_l;
          break;
        case B00000100:
          anim = anim_walk_r;
          break;
        case B00000001:
          anim = anim_walk_l_u;
          break;
        case B00000010:
          anim = anim_walk_l_d;
          break;
        case B00000101:
          anim = anim_walk_r_d;
          break;
        case B00000110:
          anim = anim_walk_r_u;
          break;
      }
      break;
    case AN_LAND:
      break;
    case AN_JUMP:
      break;
    default:
      anim = NULL;  
  }

  if (anim) {
    frame_count = pgm_read_byte_near(anim);
  }

}


void Player::processAnim() {
  
  switch (anim_action) {
    case AN_WALK:
      break;
  }
}


void Player::processControls() {
  if (command_flags == 0)
    return;
    
  if (landed) {
    if ((command_flags & CF_LEFT) && can_move) {
      dir = -1;
      if ((cells & 0x10) == 0) {  // stop cell is free
        if (((cells & 0x0F) == 0x07) | ((cells & 0x0F) == 0x05))  // climbing up. Less cells checked, otherwise lifting in air after slope end
          if ((cells & 0x40) > 0) // if head cell hit
            return;   // prevent from moving
          else
            y -= walk_speed;
        if (((cells & 0x0F) == 0x02) | ((cells & 0x0F) == 0x0A) | ((cells & 0x0F) == 0x0B))
          y += walk_speed;
        x -= walk_speed;
        switchAnim(AN_WALK);
      } else
        switchAnim(AN_STAND);
      
    };
    if ((command_flags & CF_RIGHT) && can_move) {
      dir = 1;
      if ((cells & 0x20) == 0) {  // stop cell is free
        if (((cells & 0x0F) == 0x0A) | ((cells & 0x0F) == 0x0B))  // climbing up. Less cells checked, otherwise lifting in air after slope end
          if ((cells & 0x40) > 0) // if head cell hit
            return;   // prevent from moving
          else
            y -= walk_speed;
        if (((cells & 0x0F) == 0x01) | ((cells & 0x0F) == 0x07) | ((cells & 0x0F) == 0x05))
          y += walk_speed;
        x += walk_speed;
        switchAnim(AN_WALK);
      } else
        switchAnim(AN_STAND);
    };
    if ((command_flags & CF_JUMP) || want_jump) {
      if (!can_move) {
        want_jump = true;
        return;
      }
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
        want_jump = false;
        dy = -jump_speed_y;
        dx = jump_speed_x * dir;
        switchAnim(AN_JUMP);
      };
    };
  };
  
}


void Player::process() {
  commandFromKeys();
  processControls();

  if (landed) {
    if (command_flags == 0) 
      switchAnim(AN_STAND);
  }
  
  y += dy;
  x += dx;

  checkCells(); // before correcting player position after collision

  dy = min(dy + GRAVITY, MAX_SPEED); // gravity
  o_landed = landed;
  landed = (((cells & 0x03) > 0) & ~(((cells & 0x3F) == 0x15) | ((cells & 0x3F) == 0x2A)));
  landed &= (dy >= 0); // falling
  if (landed && (!o_landed))
    switchAnim(AN_LAND);

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
        y += (1 << FBITS);
      };
    };
    if (dx != 0) {
      if (((cells & 0x28) > 0) | ((cells & 0x14) > 0)) {
        x -= (1 << FBITS) * SIGN(dx);
        dx = -dx;
      };
    };
  }

  // unstuck
  while ((cells & 0x0C) == 0x0C) {
    y -= (1 << FBITS);
    checkCells(); // after corrections and before player moving
  };

}


Player player;

void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(30);
  player.x = 110 << FBITS;
  player.y = 40 << FBITS;
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
  player.processAnim();
  player.draw();

  arduboy.display();

}
