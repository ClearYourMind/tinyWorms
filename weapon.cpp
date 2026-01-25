#include "weapon.h"
#include "fixedmath.h"


void WeaponSystem::draw(WeaponState& state, int8_t x, int8_t y) {
  if (!state.shown) return;

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


void WeaponSystem::update(WeaponState& state, int8_t dir) {

}


WeaponState WeaponSystem::newWeapon(WeaponType type) {
  return {
    .model = new Model(
      weapon_list[type].model_arr,
      weapon_list[type].vertex_count
    ),
    .type = type,
    .angle = 0,
    .scale = 1 << FBITS,
    .anim_state = 0,
    .shown = false
  };
}
