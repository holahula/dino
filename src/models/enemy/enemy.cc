#include "enemy.h"
#include "./../tower/tower.h"

#include <algorithm>

using namespace std;

Enemy::Enemy(int hp) : hp(hp), isFrozen(0) {}

int Enemy::getHP() const {
    return hp;
}

void Enemy::decFrozen() {
    isFrozen = max(isFrozen--, 0);
}

int Enemy::getFrozen() const {
    return isFrozen;
}

void Enemy::notify(Tower * tower){
    pair<char, int> type = tower->getType();

    if(type.first == 'D') hp -= type.second;
    else if (type.first == 'F') isFrozen = type.second;
}

void Enemy::observeTowers(vector<Tower*> towers){
    for(int i = 0; i < towers.size(); i++){
        towers[i]->attach(this);
    }
}

void Enemy::removeTowers(){
    for(int i = 0; i < subjects.size(); i++){
        subjects[i]->detach(this);
    }
    subjects.clear();
}

Enemy::~Enemy(){
    for(int i = 0; i < subjects.size(); i++){
        subjects[i]->detach(this);
    }
}