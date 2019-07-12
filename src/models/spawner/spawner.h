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

class Spawner {
    private:
        static const int INVISIBLE_THRESHOLD = 10;
        static const int REGENERATIVE_THRESHOLD = 20;

        // types of mobs you can spawn
        bool regen, invisible;

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

        // state update f(n)s
        void playerUpdate(int);
        void enemyUpdate(double);
        void updatePoints();
    
        void updateEnemies();
        void updateHealth();
        void updateArmor();

        void updateTraining(int);

        bool isBossLevel(int);

        int hordeSize();

        void updateEnemyType();

        Enemy* generateEnemy(int, int, bool, int);


        // enemy generation
        std::lognormal_distribution<> generateLogDistribution(double, double);
        std::bernoulli_distribution generateBernoulliDistribution(double);
        std::uniform_int_distribution<> generateUniformDistribution(int, int);
    public:
        Spawner();
        ~Spawner();

        void updateState(int, int, double);

        std::vector<Enemy*> generateEnemies(int);
        int getBonusGold();

};

#endif