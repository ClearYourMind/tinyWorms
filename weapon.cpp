#include "weapon.h"
#include "fixedmath.h"

const WeaponData WeaponSystem::weapon_list[WEAPON_TYPE_COUNT] = {
  {
  .model_arr = w_pistol,
  .vertex_count = 8,
  .chargeable = false,
  .expl_radius = 1,
  .power = (1 << FBITS),
  .dir_offset_x = {2, 5}
  }
};


void WeaponSystem::draw(WeaponState& state, int8_t x, int8_t y) {
  if (!state.shown) return;
  x = x + weapon_list[state.type].dir_offset_x[state.dir == 1];
  y = y + 10;
  state.model->drawFill(x, y);
  state.model->drawOutline(x, y);
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

}


void WeaponSystem::update(WeaponState& state) {
  int16_t _scale_y = state.scale * state.dir;
  uint8_t _angle = (MAXANGLESEC + state.angle + ((1 - state.dir) << 3)) % MAXANGLESEC;
  state.model->transform(_angle, state.scale, _scale_y);
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

  return _result;
}
