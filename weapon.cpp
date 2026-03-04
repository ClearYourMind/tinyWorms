#include "weapon.h"
#include "fixedmath.h"

const WeaponData WeaponSystem::weapon_list[WEAPON_TYPE_COUNT] = {
  {
  .model_arr = w_pistol,
  .vertex_count = 8,
  .chargeable = false,
  .expl_radius = 1,
  .power = (1 << FBITS),
  .dir_offset_x = {2, 5},
  .offset_y = 10,
  .shot_point_offset = {10, -3},
  .shot_period = 20
  }
};


void WeaponSystem::draw(WeaponState& state, int8_t x, int8_t y) {
  if (!state.shown) return;
  x = x + weapon_list[state.type].dir_offset_x[state.dir == 1];
  y = y + weapon_list[state.type].offset_y;
  state.model->drawFill(x, y);
  state.model->drawOutline(x, y);
  arduboy.drawPixel(state.shot_point[0]-1, state.shot_point[1],   counter % 2);
  arduboy.drawPixel(state.shot_point[0]+1, state.shot_point[1],   counter % 2);
  arduboy.drawPixel(state.shot_point[0],   state.shot_point[1]-1, counter % 2);
  arduboy.drawPixel(state.shot_point[0],   state.shot_point[1]+1, counter % 2);
}


void WeaponSystem::show(WeaponState& state) {
  if (state.shown) return;
  state.shown = true;
}


void WeaponSystem::hide(WeaponState& state) {
  if (!state.shown) return;
  state.shown = false;
}


void WeaponSystem::shoot(WeaponState& state) {
  if (!state.shown) return;
  if ((counter - state.last_counter) >= weapon_list[state.type].shot_period) {
    
  };

}


void WeaponSystem::update(WeaponState& state, int16_t player_x, int16_t player_y) {
  int16_t _scale_y = state.scale * state.dir;
  uint8_t _angle = state.dir == 1 ? (MAXANGLESEC + state.angle) % MAXANGLESEC : (MAXANGLESEC >> 1) - state.angle;
  state.model->transform(_angle, state.scale, _scale_y);
  // update shot point
  int16_t _s, _c;
  getSinCos(_angle, &_s, &_c);

  int16_t _shot_point_offset_x = (int16_t)weapon_list[state.type].shot_point_offset[0] << FBITS;
  int16_t _shot_point_offset_y = (int16_t)weapon_list[state.type].shot_point_offset[1] << FBITS;
  state.shot_point[0] = (fmul(_shot_point_offset_x, _c) - fmul(_shot_point_offset_y * state.dir, _s)) >> FBITS;
  state.shot_point[1] = (fmul(_shot_point_offset_x, _s) + fmul(_shot_point_offset_y * state.dir, _c)) >> FBITS;
  state.shot_point[0] += player_x >> FBITS;
  state.shot_point[1] += player_y >> FBITS;
  state.shot_point[0] += weapon_list[state.type].dir_offset_x[state.dir == 1];
  state.shot_point[1] += weapon_list[state.type].offset_y;
}


WeaponState WeaponSystem::newWeapon(WeaponType type) {
  WeaponState _result;
  _result.model = new Model(
    weapon_list[type].model_arr,
    weapon_list[type].vertex_count
  );
  _result.type = type;
  _result.angle = 0;
  _result.scale = 1 << FBITS;
  _result.anim_state = 0;
  _result.shown = false;
  _result.last_counter = counter;
  return _result;
}
