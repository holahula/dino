#include "invisible.h"
#include "./../enemy.h"

using namespace std;

InvisibleEnemy::InvisibleEnemy(int hp) : Enemy::Enemy(hp){
    targetable = false;
}

void InvisibleEnemy::updateVisibility(){
    targetable = !targetable;
}

char InvisibleEnemy::getType() const {
    return 'I';
}

void InvisibleEnemy::nextState() { 
    updateVisibility();
}

InvisibleEnemy::~InvisibleEnemy(){}