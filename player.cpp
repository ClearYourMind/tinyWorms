#include "player.h"
#include "common.h"
#include "camera.h"

Player::Player() {
  walk_speed = 1 << FBITS;
  jump_speed_y = 5 << (FBITS-1);  // 2.5
  jump_speed_x = 3 << (FBITS-1);  // 1.5
  cx = new int8_t[CELL_CHECK_COUNT];
  cy = new int8_t[CELL_CHECK_COUNT];
  dir = 1;
  can_move = false;
  want_jump = false;
}

Player::~Player() {
  delete [] cx;
  delete [] cy;
}

void Player::drawDebugOverlay() {
  uint8_t osc = counter % 2;

  uint8_t c = (counter >> 1) % CELL_CHECK_COUNT;
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
  arduboy.setCursor(24, 0);
  arduboy.print(x);
  arduboy.setCursor(24, 8);
  arduboy.print(y);

  arduboy.setCursor(60, 0);
  arduboy.print(dx);
  arduboy.setCursor(60, 8);
  arduboy.print(dy);

  // draw flags
  arduboy.setCursor(80, 0);
  arduboy.print(command_flags, HEX);
  arduboy.setCursor(80, 8);
  arduboy.print(anim_action);
  
  arduboy.setCursor(96, 0);
  arduboy.print((uint8_t)anim, HEX);
  arduboy.setCursor(96, 8);
  arduboy.print(anim_ended ? "E" : "");

  arduboy.setCursor(112, 0);
  arduboy.print(can_move ? "o" : "x");
  arduboy.setCursor(112, 8);
  arduboy.print(want_jump ? "j" : " ");
}


void Player::draw(Camera camera) {
  uint32_t _x = (x >> FBITS) - (camera.x >> FBITS);
  uint32_t _y = (y >> FBITS) - (camera.y >> FBITS);

  // draw anim frame
  uint8_t _frame = pgm_read_byte_near(anim + frame + 1);
  drawFrame(_x, _y + 8, _frame);

  if (debug_info_toggle) 
    drawDebugOverlay();
};


void Player::checkCells() {
  //update cells
  uint32_t _x = x >> FBITS;
  uint32_t _y = y >> FBITS;
  uint8_t f_screen; /// 0..3

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
  for (int8_t c=0; c<CELL_CHECK_COUNT; c++) {
    f_screen = (cx[c] >> 5) % 2;                    //  (cx / 32) mod 2 = 0..1
    f_screen = f_screen + (((cy[c] >> 4) % 2) << 1);  // ((cy / 16) mod 2) * 2 = 0..2
    cell = (field[f_screen][cy[c] % CELL_COUNT_Y] & (0x80000000 >> (cx[c] % CELL_COUNT_X))) > 0 ? 1 : 0;
    cells |= (cell << c);
  }
}


void Player::commandFromKeys() {
  // B button not used in controls by now
  command_flags = setFlagAsBool(command_flags, CF_LEFT,  arduboy.pressed(LEFT_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_RIGHT, arduboy.pressed(RIGHT_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_UP,    arduboy.pressed(UP_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_DOWN,  arduboy.pressed(DOWN_BUTTON));
  command_flags = setFlagAsBool(command_flags, CF_JUMP,  arduboy.pressed(A_BUTTON));
//  command_flags = setFlagAsBool(command_flags, CF_SHOOT, arduboy.pressed(B_BUTTON));
}


void Player::switchAnim(uint8_t _anim_action, bool forced = false) {
  uint8_t anim_flags = 0;
  anim_flags = setFlagAsBool(anim_flags, AF_DIAG_DOWN, ((cells & 0x0F) == 0x01) | ((cells & 0x0F) == 0x05) | ((cells & 0x0F) == 0x07));
  anim_flags = setFlagAsBool(anim_flags, AF_DIAG_UP,   ((cells & 0x0F) == 0x02) | ((cells & 0x0F) == 0x0A) | ((cells & 0x0F) == 0x0B));
  anim_flags = setFlagAsBool(anim_flags, AF_RIGHT,     dir == 1);

  // some anims block switching
  if (!forced)
  switch (anim_action) {
    case AN_LAND:
    case AN_JUMP:
      return;
  }
  anim_action = _anim_action;
  
  // these switch statements can be replaced with 2D array
  switch (_anim_action) {
    case AN_STAND:
      anim = anim_stand_set[anim_flags];
      break;
    case AN_WALK:
      anim = anim_walk_set[anim_flags];
      break;
    case AN_LAND:
      frame = 0;
      anim_ended = false;
      can_move = false;
      if (dir == -1)
        anim = anim_jumpland_l;
      else
        anim = anim_jumpland_r;
      break;
    case AN_JUMP:
      frame = 0;
      anim_ended = false;
      can_move = false;
      if (dir == -1)
        anim = anim_jumpstart_l;
      else
        anim = anim_jumpstart_r;
      break;
    case AN_FALL:
      if (dir == -1)
        anim = anim_jump_l;
      else
        anim = anim_jump_r;
      break;
    default:
      anim = NULL;  
  }

  if (anim) {
    frame_count = pgm_read_byte_near(anim);
  }

}


void Player::processAnim() {  
  if (arduboy.everyXFrames(3))
    frame++;
   
  anim_ended = (frame == frame_count);
  frame = frame % frame_count;
 
  switch (anim_action) {
    case AN_WALK:
      if (!landed) {
        frame = 0;
        switchAnim(AN_FALL, true);
      }
    case AN_STAND:
      break;
    case AN_LAND:
      if (anim_ended) {
        switchAnim(AN_STAND, true);
        can_move = true;
      }
      break;
    case AN_JUMP:
      if (anim_ended) {
        switchAnim(AN_FALL, true);
      }
      if (landed)
        can_move = true;
      break;
    case AN_FALL:
      if (landed)
        can_move = true;
      break;
  }
}


void Player::processControls() {
    
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
      } else
        want_jump = false;
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
  if (landed && (!o_landed)) {
    switchAnim(AN_LAND);
  }
  
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
      if ((cells & 0x30) > 0) {
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
