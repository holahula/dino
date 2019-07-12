#include "./../enemy/enemy.h"
#include "./../enemy/basic/basic.h"
#include "./../enemy/invisible/invisible.h"
#include "./../enemy/regenerative/regenerative.h"
#include "spawner.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <map>
#include <random>
#include <utility>
#include <chrono>

using namespace std;

Spawner::Spawner(): regen(false), invisible(false), training(true), difficulty(10), status(1), gold(1), spawn(1), statusMultiplier(1), goldMultiplier(1), spawnMultiplier(1), health(1), armor(1) {}
Spawner::~Spawner() {}

void Spawner::playerUpdate(int hpLost){
    if(hpLost > 10) {
        statusMultiplier -= 0.2;
        spawnMultiplier -= 0.16;
        goldMultiplier += 0.15;
        status -= 4;
        spawn -= 4;
    } else if (hpLost >= 8) {
        statusMultiplier -= 0.17;
        spawnMultiplier -= 0.14;
        goldMultiplier += 0.12;
        status -= 3;
        spawn -= 3;
    } else if (hpLost >= 4) {
        statusMultiplier -= 0.12;
        spawnMultiplier -= 0.10;
        goldMultiplier += 0.09;
        status -= 2;
        spawn -= 2;
    } else if (hpLost >= 1) {
        statusMultiplier -= 0.1;
        spawnMultiplier -= 0.08;
        goldMultiplier += 0.07;
    } else {
        statusMultiplier += 0.07;
        spawnMultiplier += 0.05;
        goldMultiplier -= 0.05;
        status += 1;
        spawn += 1;
    }
}

void Spawner::enemyUpdate(double enemyHP){
    if(enemyHP >= 0.20) {
        statusMultiplier -= 0.10;
        spawnMultiplier -= 0.075;
        goldMultiplier += 0.075;
        difficulty -= 8;
    } else if (enemyHP >= 0.10) {
        statusMultiplier -= 0.07;
        spawnMultiplier -= 0.05;
        goldMultiplier += 0.03;
        difficulty -= 6;
    } else if (enemyHP >= 0.04) {
        statusMultiplier -= 0.05;
        spawnMultiplier -= 0.04;
        goldMultiplier += 0.03;
        difficulty -= 4;
    } else if (enemyHP > 0) {
        statusMultiplier -= 0.04;
        spawnMultiplier -= 0.03;
        goldMultiplier += 0.02;
        difficulty -= 2;
    } else {
        statusMultiplier += 0.06;
        spawnMultiplier += 0.05;
        goldMultiplier -= 0.04;
        difficulty += 4;
    }
}

void Spawner::updatePoints(){
    statusMultiplier = max(0.01, statusMultiplier);
    goldMultiplier = max(0.01, goldMultiplier);
    spawnMultiplier = max(0.01, spawnMultiplier);

    status = max(0.01, status * statusMultiplier);
    gold = max(0.01, gold * goldMultiplier);
    spawn = max(0.01, spawn * spawnMultiplier);
}

void Spawner::updateHealth(){
    health = max(1.0, health * status + (20 * difficulty/100));
}

void Spawner::updateArmor(){
    armor = max(1.0, armor * (status / 4) + (20 * difficulty/100));
}

void Spawner::updateEnemyType(){
    (armor >= (double)INVISIBLE_THRESHOLD) ? invisible = true : invisible = false;
    (armor >= (double)REGENERATIVE_THRESHOLD) ? regen = true : regen = false;
}

void Spawner::updateEnemies(){
    updateHealth();
    updateArmor();
    updateEnemyType();
}

void Spawner::updateTraining(int round){
    status++;
    spawn++;

    if(status == 19 || spawn == 19){
        training = !training;
    }
}

void Spawner::updateState(int round, int hpLost, double enemyHP){
    // cout << "state being updated!" << endl;
    // cout << round << " " << hpLost << " " << enemyHP << endl;
    // cout << difficulty << " " << status << " " << gold << " " << spawn << endl;
    // cout << statusMultiplier << " " << goldMultiplier << " " << spawnMultiplier << endl;
    
    playerUpdate(hpLost);
    enemyUpdate(enemyHP);
    updatePoints();
    updateEnemies();
    if(training && round % 2 == 0) {
        updateTraining(round);
    }

    // cout << difficulty << " " << status << " " << gold << " " << spawn << endl;
    // cout << statusMultiplier << " " << goldMultiplier << " " << spawnMultiplier << endl;
}

bool Spawner::isBossLevel(int round) {
    return round % 5 == 0;
}

int Spawner::hordeSize() {
    return max(20.0, 20 + (30 * difficulty / 100) + spawn);
}

lognormal_distribution<> Spawner::generateLogDistribution(double max, double min) {
    lognormal_distribution<> d(max, min);
    return d;
}

bernoulli_distribution Spawner::generateBernoulliDistribution(double ratio) {
    bernoulli_distribution d(ratio);
    return d;
}

uniform_int_distribution<> Spawner::generateUniformDistribution(int min, int max) {
    uniform_int_distribution<> d(min, max);
    return d;
}

Enemy* Spawner::generateEnemy(int hp, int round, bool invis, int regenerative){
    // cout << round << " "<< hp << " " << invis << " " << regenerative << endl;
    // cout << regen << invisible << endl;
    if(!regen && !invisible){
        return new BasicEnemy(hp);
    } else if (!regen) {
        if(invis) return new InvisibleEnemy(hp);
        else return new BasicEnemy(hp);   
    } else {
        if(regenerative >= 3) return new RegenerativeEnemy(hp, 1);
        else if (regenerative == 2) return new InvisibleEnemy(hp);
        else return new BasicEnemy(hp);
    }
}

int round_(double n){
    return round(n);
}

vector<Enemy*> Spawner::generateEnemies(int round){
    vector<Enemy*> enemies;

    int maxHP = health;
    int size = 0;

    if(round == 1) size = 20;
    else size = hordeSize();
    
    bool boss = isBossLevel(round);

    if(boss) {
        size += (int)ceil((double)size * (double)round / 50.0);
        maxHP += (int)ceil((double)health * (double)round / 50.0);
    }

    unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed1);

    lognormal_distribution<> logDist = generateLogDistribution(log(maxHP), 1.0);
    bernoulli_distribution bernoulliDist = generateBernoulliDistribution((double)round/50.0);
    uniform_int_distribution<> uniformDist = generateUniformDistribution(1, 4);

    for(int i = 0; i < size; i++){
        Enemy* e = generateEnemy(max(1, maxHP-round_(logDist(gen))), round, bernoulliDist(gen), uniformDist(gen));
        enemies.push_back(e);
    }

    return enemies;
}

int Spawner::getBonusGold(){
    return (int)max(0.0, (50.0 * (double)difficulty/100.0) + (double)gold);
}

