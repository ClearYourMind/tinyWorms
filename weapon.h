#ifndef WEAPON_H
#define WEAPON_H

#include "models.h"
#include "model.h"

enum WeaponType {
  WEAPON_PISTOL,
  WEAPON_TYPE_COUNT
};

struct WeaponData {
  const uint8_t* model_arr;
  uint8_t vertex_count;
  bool chargeable;
  uint8_t expl_radius;
  uint16_t power;
};

struct WeaponState {
  Model* model;
  uint8_t type;
  uint8_t angle;
  uint8_t scale;
  uint8_t anim_state;
  bool shown;
};

class WeaponSystem {
  private:
    static const WeaponData weapon_list[WEAPON_TYPE_COUNT] = {
      {
      .model_arr = w_pistol,
      .vertex_count = 8,
      .chargeable = false,
      .expl_radius = 1,
      .power = (1 << FBITS)
      }
    };

  public:
    static void draw(WeaponState& state, int8_t x, int8_t y);
    static void show(WeaponState& state);
    static void hide(WeaponState& state);
    static void shoot(WeaponState& state);
    static void update(WeaponState& state, int8_t dir);
    static WeaponState newWeapon(WeaponType type);

};

#endif
