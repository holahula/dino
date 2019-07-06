#include "state.h"

State::State(int money, int HP) : money{money}, HP{HP} {}

void State::earn(int amount) {
  money += amount;
}
void State::spend(int amount) {
  money -= amount;
}

void State::loseHP(int amount) {
  hp -= amount;
}

void State::addTower(Tower tower) {
  towers.emplace_back(tower);
}

State::~State() {}