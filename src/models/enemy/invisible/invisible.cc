#include "invisible.h"
#include "./../enemy.h"

using namespace std;

InvisibleEnemy::InvisibleEnemy(int hp) : Enemy::Enemy(hp), visibility(false){}

void InvisibleEnemy::updateVisibility(){
    visibility = !visibility;
}

char InvisibleEnemy::getType() const {
    return 'I';
}