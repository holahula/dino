#ifndef STATE_H_
#define STATE_H_

#include "./../enemy/enemy.h"
#include "./../tower/tower.h"
#include "./../shop/shop.h"

#include <vector>

class State {
    int hp, money, round;

    Shop * shop;
    std::vector<Tower*> towers;
    std::vector<Enemy*> enemies;

    public:
        static const int MAX_LEVEL = 10;

        State();
        ~State();


        bool loseHP(int);
        void incrementMoney(int);
        // void spend(int);

        void addTower(Tower*);

        void start();

        // LEVEL FUNCTIONS
        void startRound();
        int constructEnemies(int);
        void nextFrame();
        void updateState(int, int);

};

#endif