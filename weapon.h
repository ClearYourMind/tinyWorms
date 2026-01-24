#ifndef WEAPON_H
#define WEAPON_H

#include "player.h"
#include "camera.h"
#include "models.h"
#include "model.h"

class Weapon {
  public:
    Model* model;
    uint8_t angle;

    void draw(int8_t x, int8_t y, int8_t dir);
    void show();
    void hide();
    void update(uint8_t angle, int8_t dir);

    Weapon(Model* _model);
    ~Weapon();
};

#endif
