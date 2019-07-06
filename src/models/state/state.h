#ifndef STATE_H_
#define STATE_H_

#include <vector>
#include "../enemy/enemy.h"
#include "../tower/tower.h"

class State {
  int money;
  int hp;
  std::vector<Tower*> towers;
  std::vector<Enemy*> enemies;

 public:
  State(int money = 100, int HP = 100);
  ~State();
  void earn(int);
  void spend(int);
  void loseHP(int);
  void addTower(Tower tower);
};

#endif