#include "./../shop/shop.h"

#include "state.h"

State::State() : money(10), hp(100), shop(){}
State::~State() {
    for(int i = 0; i < towers.size(); i++) delete towers[i];
    for(int i = 0; i < enemies.size(); i++) delete enemies[i];
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

void State::addTower(Tower* tower) {
    towers.push_back(tower);
}

void State::constructEnemies(int round) {

}

void State::nextFrame(){}

void State::updateState(int hp, int round){}

void State::startRound(){
    State::constructEnemies(round);
    // how to push enemies into the map
    while(enemies.size() != 0){
        State::nextFrame();
    }

    State::updateState(hp, round);
}

void State::start(){}