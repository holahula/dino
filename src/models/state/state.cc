#include "./../shop/shop.h"
#include "./../map/map.h"
#include "./../tile/path/pathTile.h"
#include "state.h"

#include <vector>
#include <algorithm>

using namespace std;

State::State() : money(10), hp(100), shop(new Shop()), map(new Map()){}

State::~State() {
    for(auto &tower : towers) delete tower;
    for(auto &enemy : enemies) delete enemy;
}  

void State::incrementMoney(int amount) {
    money += amount;
}

// void State::spend(int amount) {
//     money -= amount;
// }

bool State::loseHP(int amount) {
    if(hp-amount <= 0) return false;

    hp -= amount;
    return true;
}

void State::addTower(Tower* t) {
    towers.push_back(t);
}

void State::addEnemy(Enemy* e){
    enemies.push_back(e);
}

void State::removeEnemy(Enemy* enemy){
    enemies.erase(remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    delete enemy;
}

// based on round, construct enemies -> insert them into state enemy vector
int State::constructEnemies(int round) {
    return 1;
}

int State::totalHPLost(vector<Enemy*> enemies){
    int total = 0;
    for(auto &e : enemies) {
        total += e->getHP();
    }
    return total;
}

bool State::moveEnemies(int frame, int sz){
    vector<Enemy*> escaped = map->nextFrame();

    if(!loseHP(totalHPLost(escaped))){
        return false;
    }

    for(auto & enemy: escaped){
        removeEnemy(enemy);
    }

    if (frame <= sz){
        map->insertEnemy(enemies[frame - 1]);
    }

    return true;
}

void State::processFrame(){
    // shoot enemies
    for(auto &tower : towers){
        // TODO: currently shoots all towers in vicinity, need to only shoot 1
        // TODO: NEED TO GAIN MONEY WHEN SHOOTING TOWERS
        tower->notifyObservers(tower);
    }

    // delete dead enemies
    for(auto&enemy : enemies) {
        if(enemy->getHP() <= 0) {
            removeEnemy(enemy);
        }
    }
}

void State::updateState(int hp, int round){
    if(hp <= 0) {
        // TODO: handle death
        cout << "dead on round " << round << endl;
    }
}

void State::startRound(){
    int frame = 1;
    int sz = constructEnemies(round);

    // round while loop 
    while(enemies.size() != 0){
        // dead
        if(!moveEnemies(frame, sz)){
            break;
        }

        processFrame();
        // nextFrame();
        frame++;
    }
    // round over -> either all enemies killed, or died
    updateState(hp, round);
}

// map -> next frame returns 2 vectors of enemies
// 1st is dead 
// 2nd is reached end 
// need to delete all of these 