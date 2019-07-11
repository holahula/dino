#ifndef STATE_H_
#define STATE_H_

#include "./../enemy/enemy.h"
#include "./../map/map.h"
#include "./../shop/shop.h"
#include "./../tower/tower.h"

#include <stack>
#include <vector>

/*
TODO: money management functions,
    creating towers + enemies
    detach and attach enemies every frame?

 game functionality is done?
 */

class State {
  int hp, money, round;

  Shop *shop;
  Map *map;

  std::vector<Tower *> towers;
  std::vector<Enemy *> enemies;
  std::stack<Enemy *> pendingEnemies;

  bool surviveDmg(int);
  void incrementMoney(int);
  // void spend(int);

  void addTower(Tower *);
  void addEnemy(Enemy *);
  void removeEnemy(Enemy *);

  int constructEnemies(int);

  int totalHPLost(std::vector<Enemy *>);

  bool moveEnemies(int, int);

  bool preFrame(int, int);

  void processFrame();
  void postFrame();

  void getMoneyTowerIncome();
  void updateState(int, int);

 public:
  static const int MAX_ROUND = 10;

  State();
  ~State();

  void displayMap();
  bool buyTower(char, int, int);
  bool upgradeTower(int, int);

  void startRound();
  
  Map& getMap();
  int getMoney();
  int getHp();
  int getRound();
};

#endif