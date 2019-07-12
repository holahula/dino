#include "./../enemy/enemy.h"
#include "./../enemy/basic/basic.h"

#include "spawner.h"

#include <vector>

using namespace std;

Spawner::Spawner(): difficulty(10), status(1), gold(1), spawn(1), statusMultiplier(1), goldMultiplier(1), spawnMultiplier(1) {}
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
    status *= statusMultiplier;
    spawner *= spawnerMultiplier;
    gold *= goldMultiplier;
}

void Spawner::updateState(int hpLost, double enemyHP){
    playerUpdate(hpLost);
    enemyUpdate(enemyHP);
    updatePoints();
}

bool Spawner::isBossLevel(int round) {
    return round % 5 == 0;
}

int Spawner::hordeSize() {
    return max(20, 20 + (30 * difficulty / 100) + spawn)
}

vector<Enemy*> Spawner::generateEnemies(){
    int amount = hordeSize()
    vector<Enemy*> enemies;
    enemies.push_back(new BasicEnemy(1));

    return enemies;
}

int Spawner::getBonusGold(){
    return (50 * difficulty/100) + gold;
}
