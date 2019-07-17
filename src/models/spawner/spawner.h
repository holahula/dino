#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "./../enemy/enemy.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <map>
#include <random>
#include <utility>

struct SpawnerImpl {
    // adaptive level generation | types of mobs you can spawn
    bool adaptive, regen, invisible;
    /*
    to gauge the skill of the user,
    there is a training mode until the game becomes hard enough
    */
    bool training;
    // points
    double difficulty, status, gold, spawn;
    //mutipliers
    double statusMultiplier, goldMultiplier, spawnMultiplier;
    // enemy traits
    double health, armor;

    const int INVISIBLE_THRESHOLD = 10;
    const int REGENERATIVE_THRESHOLD = 20;
    const int MAXHP_RATIO = 10;

    public: 
        SpawnerImpl(bool);
        ~SpawnerImpl();
};

class Spawner {
    private:
        SpawnerImpl* p;

        // state update f(n)s
        void playerUpdate(int);
        void enemyUpdate(double);
        void updatePoints();
    
        void updateEnemies();
        void updateHealth();
        void updateArmor();

        void printStats();

        void updateTraining(int);

        bool isBossLevel(int);

        int hordeSize();

        void updateEnemyType();

        Enemy* generateEnemy(int, int, bool, int);
        Enemy* generateEasyEnemy(int, bool, int);

        // enemy generation
        std::lognormal_distribution<> generateLogDistribution(double, double);
        std::bernoulli_distribution generateBernoulliDistribution(double);
        std::uniform_int_distribution<> generateUniformDistribution(int, int);
    public:
        Spawner(bool);
        ~Spawner();

        void updateState(int, int, double);

        std::vector<Enemy*> generateEnemies(int);
        int getBonusGold();

};

#endif