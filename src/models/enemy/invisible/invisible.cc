#include "invisible.h"
#include "./../enemy.h"

using namespace std;

InvisibleEnemy::InvisibleEnemy(int hp) : Enemy::Enemy(hp), visible(false){}

void InvisibleEnemy::updateVisibility(){
    visible = !visible;
}

char InvisibleEnemy::getType() const {
    return 'I';
}

void InvisibleEnemy::nextState() { 
    updateVisibility();
}

void InvisibleEnemy::notify(Tower* tower) {
    if (visible) {
        Enemy::notify(tower);
    }
}

InvisibleEnemy::~InvisibleEnemy(){}