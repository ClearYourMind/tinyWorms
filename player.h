#ifndef PLAYER_H
#define PLAYER_H

#include "Arduino.h"
#include "Arduboy2.h"
#include "fixedmath.h"
#include "animations.h"

#include "common.h"
#include "camera.h"

#define CELL_CHECK_COUNT 7      // cells for checking collision

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

// basic animations to switch to (anim_action)
#define AN_STAND  1
#define AN_WALK   2
#define AN_JUMP   3
#define AN_FALL   4
#define AN_LAND   5


extern uint16_t counter;
extern Arduboy2 arduboy;
extern Sprites sprites;
extern bool debug_info_toggle;

extern uint32_t *field_ptr[4];

extern uint8_t setFlagAsBool(uint8_t flags, uint8_t flag, bool bool_value);
extern void drawFrame(int8_t x, int8_t y, uint8_t frame_id);
extern void setCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y, bool value);
extern bool getCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y);


// anim set based on anim_flags
uint8_t* const anim_stand_set[] = {
  anim_stand_l,
  anim_stand_l_u,
  anim_stand_l_d,
  NULL,
  anim_stand_r,
  anim_stand_r_d,
  anim_stand_r_u
};
uint8_t* const anim_walk_set[] = {
  anim_walk_l,
  anim_walk_l_u,
  anim_walk_l_d,
  NULL,
  anim_walk_r,
  anim_walk_r_d,
  anim_walk_r_u
};

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
    uint8_t command_flags;
    uint8_t frame;
    uint8_t frame_count;
    uint8_t anim_action;
    bool anim_ended;
    uint8_t* anim; // array

    Player();
    ~Player();
    void draw(Camera camera);
    void drawDebugOverlay();
    void process();
    void processControls();
    void processAnim();
    void switchAnim(uint8_t _anim_action, bool forced = false);
    void checkCells();
    void commandFromKeys();
};


#endif
