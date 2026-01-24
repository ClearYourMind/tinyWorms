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
    int16_t scale;

    void draw(int8_t x, int8_t y);
    void show();
    void hide();
    void shoot();
    void update(int8_t dir);

    Weapon(Model* _model);
    ~Weapon();
};

#endif
