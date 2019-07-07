#include "./../shop/shop.h"
#include "./../map/map.h"
#include "./../tile/path/pathTile.h"
#include "./../enemy/basic/basic.h"
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
    for(int i = 0; i < 10; i++){
        addEnemy(new BasicEnemy(1));
    }
    return 10;
}

int State::totalHPLost(vector<Enemy*> enemies){
    int total = 0;
    for(auto &e : enemies) {
        total += e->getHP();
    }
    return total;
}

bool State::moveEnemies(int frame, int size){
    vector<Enemy*> escaped = map->nextFrame();

    if(!loseHP(totalHPLost(escaped))){
        return false;
    }

    for(auto & enemy: escaped){
        removeEnemy(enemy);
    }

    if (frame <= size){
        map->insertEnemy(enemies[frame - 1]);
    }

    return true;
}

void State::processFrame(){
    // shoot enemies
    map->attachAllEnemies();

    for(auto &tower : towers){
        pair<int, int> type = tower->getType();
        if(type.first == 'D'){
            incrementMoney(type.second);
        }

        tower->notifyObservers(tower);
    }

    for (auto &enemy : map->removeDeadEnemies()) {
        removeEnemy(enemy);
    }
}

// prepares for the next frame, detachs all the enemies from their respective towers
void State::nextFrame(){
    map->detachAllEnemies();
}

void State::updateState(int hp, int round){
    if(hp <= 0) {
        // TODO: handle death, clean up everything
        cout << "dead on round " << round << endl;
    } else if(round == MAX_ROUND){
        cout << "winner winner chicken dinner" << endl;
    }
}

void State::startRound(){
    int frame = 1;
    int size = constructEnemies(round);

    // round while loop 
    while(enemies.size() != 0){
        // dead
        if(!moveEnemies(frame, size)){ 
            break;
        }
        processFrame();
        nextFrame();
        frame++;
    }
    updateState(hp, round);
}

// map -> next frame returns 2 vectors of enemies
// 1st is dead 
// 2nd is reached end 
// need to delete all of these 