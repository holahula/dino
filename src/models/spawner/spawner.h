#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "./../enemy/enemy.h"
// #include "./../state/state.h"

#include <vector>

class Spawner {
    private:
        // points
        double difficulty, status, gold, spawn;

        //mutipliers
        double statusMultiplier, goldMultiplier, spawnMultiplier;

        void playerUpdate(int);
        void enemyUpdate(double);
        void updatePoints();

        bool isBossLevel(int);

        int hordeSize();

    public:
        Spawner();
        ~Spawner();


        std::vector<Enemy*> generateEnemies();
        void updateState(int, double);
        int getBonusGold();

};

#endif