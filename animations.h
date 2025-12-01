#ifndef ANIMATIONS
#define ANIMATIONS

#include "spritesheet.h"
#include "frames.h"
#include "avr/pgmspace.h"

// frame_count, frame ids
uint8_t const anim_stand_r[] PROGMEM =    {1, 0x00};
uint8_t const anim_walk_r[] PROGMEM =     {4, 0x01, 0x01, 0x02, 0x02};
uint8_t const anim_stand_r_u[] PROGMEM =  {1, 0x03};
uint8_t const anim_walk_r_u[] PROGMEM =   {4, 0x04, 0x04, 0x05, 0x05};
uint8_t const anim_stand_r_d[] PROGMEM =  {1, 0x06};
uint8_t const anim_walk_r_d[] PROGMEM =   {4, 0x07, 0x07, 0x08, 0x08};
uint8_t const anim_jumpstart_r[] PROGMEM ={2, 0x09, 0x0A};
uint8_t const anim_jump_r[] PROGMEM =     {2, 0x0B, 0x0C};
uint8_t const anim_jumpland_r[] PROGMEM = {3, 0x0D, 0x0E, 0x0F};
uint8_t const anim_slide_r[] PROGMEM =    {2, 0x10, 0x11};
uint8_t const anim_slide_r_u[] PROGMEM =  {2, 0x12, 0x13};
uint8_t const anim_slide_r_d[] PROGMEM =  {2, 0x14, 0x15};
uint8_t const anim_recover_r[] PROGMEM =  {5, 0x16, 0x17, 0x18, 0x19, 0x1A};

uint8_t const anim_stand_l[] PROGMEM =    {1, 0x1B};
uint8_t const anim_walk_l[] PROGMEM =     {4, 0x1C, 0x1C, 0x1D, 0x1D};
uint8_t const anim_stand_l_u[] PROGMEM =  {1, 0x1E};
uint8_t const anim_walk_l_u[] PROGMEM =   {4, 0x1F, 0x1F, 0x20, 0x20};
uint8_t const anim_stand_l_d[] PROGMEM =  {1, 0x21};
uint8_t const anim_walk_l_d[] PROGMEM =   {4, 0x22, 0x22, 0x23, 0x23};
uint8_t const anim_jumpstart_l[] PROGMEM ={2, 0x24, 0x25};
uint8_t const anim_jump_l[] PROGMEM =     {2, 0x26, 0x27};
uint8_t const anim_jumpland_l[] PROGMEM = {3, 0x28, 0x29, 0x2A};
uint8_t const anim_slide_l[] PROGMEM =    {2, 0x2B, 0x2C};
uint8_t const anim_slide_l_u[] PROGMEM =  {2, 0x2D, 0x2E};
uint8_t const anim_slide_l_d[] PROGMEM =  {2, 0x2F, 0x30};
uint8_t const anim_recover_l[] PROGMEM =  {5, 0x31, 0x32, 0x33, 0x34, 0x35};


#endif
