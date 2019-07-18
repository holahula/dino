#include "./../enemy/enemy.h"
#include "./../enemy/basic/basic.h"
#include "./../enemy/invisible/invisible.h"
#include "./../enemy/regenerative/regenerative.h"
#include "spawner.h"

#include <limits>
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

SpawnerImpl::SpawnerImpl(bool adaptive): adaptive(adaptive), regen(false), invisible(false), training(true), difficulty(10), status(1), gold(1), spawn(1), statusMultiplier(1), goldMultiplier(1), spawnMultiplier(1), health(1), armor(1) {}

Spawner::Spawner(bool adaptive): p(new SpawnerImpl(adaptive)) {}
Spawner::~Spawner() {
    delete p;
}

void Spawner::playerUpdate(int hpLost){
    if(hpLost > 10) {
        p->statusMultiplier -= 0.2;
        p->spawnMultiplier -= 0.16;
        p->goldMultiplier += 0.15;
        p->status -= 4;
        p->spawn -= 4;
    } else if (hpLost >= 8) {
        p->statusMultiplier -= 0.17;
        p->spawnMultiplier -= 0.14;
        p->goldMultiplier += 0.12;
        p->status -= 3;
        p->spawn -= 3;
    } else if (hpLost >= 4) {
        p->statusMultiplier -= 0.12;
        p->spawnMultiplier -= 0.10;
        p->goldMultiplier += 0.09;
        p->status -= 2;
        p->spawn -= 2;
    } else if (hpLost >= 1) {
        p->statusMultiplier -= 0.1;
        p->spawnMultiplier -= 0.08;
        p->goldMultiplier += 0.07;
    } else {
        p->statusMultiplier += 0.05;
        p->spawnMultiplier += 0.03;
        p->goldMultiplier -= 0.05;
        p->status += 1;
        p->spawn += 1;
    }
}

void Spawner::enemyUpdate(double enemyHP){
    if(enemyHP >= 0.20) {
        p->statusMultiplier -= 0.10;
        p->spawnMultiplier -= 0.075;
        p->goldMultiplier += 0.075;
        p->difficulty -= 8;
    } else if (enemyHP >= 0.10) {
        p->statusMultiplier -= 0.07;
        p->spawnMultiplier -= 0.05;
        p->goldMultiplier += 0.03;
        p->difficulty -= 6;
    } else if (enemyHP >= 0.04) {
        p->statusMultiplier -= 0.05;
        p->spawnMultiplier -= 0.04;
        p->goldMultiplier += 0.03;
        p->difficulty -= 4;
    } else if (enemyHP > 0) {
        p->statusMultiplier -= 0.04;
        p->spawnMultiplier -= 0.03;
        p->goldMultiplier += 0.02;
        p->difficulty -= 2;
    } else {
        p->statusMultiplier += 0.05;
        p->spawnMultiplier += 0.03;
        p->goldMultiplier -= 0.03;
        p->difficulty += 4;
    }
}

void Spawner::updatePoints(){
    p->statusMultiplier = max(0.01, p->statusMultiplier);
    p->goldMultiplier = max(0.01, p->goldMultiplier);
    p->spawnMultiplier = max(0.01, p->spawnMultiplier);

    p->status = max(0.01, p->status * p->statusMultiplier);
    p->gold = max(0.01, p->gold * p->goldMultiplier);
    p->spawn = max(0.01, p->spawn * p->spawnMultiplier);
}

void Spawner::updateHealth(){
    p->health = max(1.0, min(numeric_limits<double>::max(), sqrt(p->health * p->status)  + (p->MAXHP_RATIO * p->difficulty/100)));
}

void Spawner::updateArmor(){
    p->armor = max(1.0, min(numeric_limits<double>::max(), p->armor * (p->status / 4) + (20 * p->difficulty/100)));
}

void Spawner::updateEnemyType(){
    (p->armor >= (double)p->INVISIBLE_THRESHOLD) ? p->invisible = true : p->invisible = false;
    (p->armor >= (double)p->REGENERATIVE_THRESHOLD) ? p->regen = true : p->regen = false;
}

void Spawner::updateEnemies(){
    updateHealth();
    updateArmor();
    updateEnemyType();
}

void Spawner::updateTraining(int round){
    p->status++;
    p->spawn++;

    if(p->status == 19 || p->spawn == 19){
        p->training = !p->training;
    }
}

void Spawner::printStats(){
    cout << "Difficulty: " << p->difficulty << endl;

    cout << "Max HP: " << p->health << endl;
    cout << "Armor: " << p->armor << endl;
    cout << "Enemies: Basic, ";
    if(p->invisible) cout << "Invisible, ";
    if(p->regen) cout << "Regenerative";
    cout << endl;

    cout << "--Base Stats--" << endl;
    cout << "Status: " << p->status << endl;
    cout << "Gold: " << p->gold << endl;
    cout << "Spawn: " << p->spawn << endl;

    cout << "--Multipliers--" << endl;
    cout << "Status Multiplier: " << p->statusMultiplier << endl;
    cout << "Gold Multiplier: " << p->goldMultiplier << endl;
    cout << "Spawn Multiplier: " << p->spawnMultiplier << endl;
}

void Spawner::updateState(int round, int hpLost, double enemyHP){
    cout << "\nPREV ROUND" << endl;
    printStats();

    playerUpdate(hpLost);
    enemyUpdate(enemyHP);
    updatePoints();
    updateEnemies();

    if(p->training && round % 2 == 0) {
        updateTraining(round);
    }

    cout << "\nNEXT ROUND" << endl;
    printStats();
}

bool Spawner::isBossLevel(int round) {
    return round % 5 == 0;
}

int Spawner::hordeSize() {
    return max(20.0, 20 + (30 * p->difficulty / 100) + p->spawn);
}

lognormal_distribution<> Spawner::generateLogDistribution(double max, double min) {
    lognormal_distribution<> d(min, max);
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
    cout << "creating enemy w/ HP: " << hp << endl;
    if(!p->regen && !p->invisible){
        return new BasicEnemy(hp);
    } else if (!p->regen) {
        if(invis) return new InvisibleEnemy(hp);
        else return new BasicEnemy(hp);   
    } else {
        if(regenerative >= 3) return new RegenerativeEnemy(hp, int(hp/8));
        else if (regenerative == 2) return new InvisibleEnemy(hp);
        else return new BasicEnemy(hp);
    }
    return nullptr;
}

Enemy* Spawner::generateEasyEnemy(int round, bool invis, int regenerative){
    cout << "creating enemy w/ HP: " << round << endl;
    if(!p->regen && !p->invisible){
        return new BasicEnemy(round);
    } else if (!p->regen) {
        if(invis) return new InvisibleEnemy(round);
        else return new BasicEnemy(round);   
    } else {
        if(regenerative >= 3) return new RegenerativeEnemy(round, int(round/8));
        else if (regenerative == 2) return new InvisibleEnemy(round);
        else return new BasicEnemy(round);
    }
    return nullptr;
}

int round_(double n){
    return round(n);
}

vector<Enemy*> Spawner::generateEnemies(int round){
    vector<Enemy*> enemies;

    int maxHP = p->health;
    int size = 0;

    if(round == 1) size = 20;
    else size = hordeSize();
    
    bool boss = isBossLevel(round);

    if(boss) {
        size += (int)ceil((double)size * (double)round / 50.0);
        maxHP += (int)ceil((double)p->health * (double)round / 50.0);
    }

    unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed1);

    lognormal_distribution<> logDist = generateLogDistribution(log(maxHP), 1.0);
    bernoulli_distribution bernoulliDist = generateBernoulliDistribution((double)round/50.0);
    uniform_int_distribution<> uniformDist = generateUniformDistribution(1, 4);

    if(p->adaptive){
        for(int i = 0; i < size; i++){
            Enemy* e = generateEnemy(max(1, (int)(maxHP-floor(logDist(gen)))), round, bernoulliDist(gen), uniformDist(gen));
            enemies.push_back(e);
        }
    } else {
        for(int i = 0; i < round; i++){
            Enemy* e = generateEasyEnemy(round, bernoulliDist(gen), uniformDist(gen));
            enemies.push_back(e);
        }
    }

    return enemies;
}

int Spawner::getBonusGold(){
    return (int)max(0.0, (50.0 * (double)p->difficulty/100.0) + (double)p->gold * 2);
}

