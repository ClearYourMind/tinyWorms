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
  int8_t dir_offset_x[2];
};

struct WeaponState {
  Model* model;
  uint8_t type;
  int8_t angle; // -8 .. 8
  int8_t dir;   // -1 .. 1
  uint8_t scale;
  uint8_t anim_state;
  bool shown;
};

class WeaponSystem {
  private:
    static const WeaponData weapon_list[WEAPON_TYPE_COUNT];

  public:
    static void draw(WeaponState& state, int8_t x, int8_t y);
    static void show(WeaponState& state);
    static void hide(WeaponState& state);
    static void shoot(WeaponState& state);
    static void update(WeaponState& state);
    static WeaponState newWeapon(WeaponType type);

};

#endif
