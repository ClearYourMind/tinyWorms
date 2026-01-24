#include "weapon.h"
#include "fixedmath.h"

Weapon::Weapon(Model* _model) {
  model = _model;

}


Weapon::~Weapon() {

}


void Weapon::show() {


}


void Weapon::hide() {


}


void Weapon::update(, int8_t dir) {
  model->transform(angle, (1 << FBITS) * dir, 1 << FBITS);
}


void Weapon::draw(int8_t x, int8_t y) {
  model->drawFill(x, y);
  model->drawOutline(x, y);
}
