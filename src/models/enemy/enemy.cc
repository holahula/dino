#include "enemy.h"
#include "./../tower/tower.h"

#include <algorithm>

using namespace std;

Enemy::Enemy(int hp) : isFrozen(0), hp(hp), targetable(true) {}

int Enemy::getHP() const {
    return hp;
}

void Enemy::decFrozen() {
    isFrozen = isFrozen > 0 ? isFrozen-1 : 0;
}

int Enemy::getFrozen() const {
    return isFrozen;
}

char Enemy::getType() {
    return getType_();
}

void Enemy::nextState() {
    nextState_();
}

int Enemy::notify(Tower * tower) {
    return notify_(tower);
}


int Enemy::notify_(Tower * tower){
    pair<char, int> type = tower->getType();
    if(type.first == 'D') {
        int prevHP = hp;
        hp = max(hp - type.second, 0);
        return prevHP-hp;
    }
    else if (type.first == 'F') {
        if (!isFrozen) {
            isFrozen = type.second;
        }
    }
    return 0;
}

void Enemy::observeTowers(vector<Tower*> towers){
    for(size_t i = 0; i < towers.size(); ++i) {
        towers[i]->attach(this);
    }
}

void Enemy::removeTowers(){
    for(size_t i = 0; i < subjects.size(); ++i){
        subjects[i]->detach(this);
    }
    subjects.clear();
}

bool Enemy::isTargetable() const {
    return targetable;
}

ostream& operator<<(std::ostream& out, Enemy& enemy) {
    out << "type: " << enemy.getType() << ", hp: " << enemy.getHP() << ", frozen: ";
    out << (enemy.getFrozen() > 0 ? "Yes" : "No") << ", targetable: "<< (enemy.isTargetable() ? "Yes" : "No") << endl;
    return out;
}

Enemy::~Enemy(){
    for(size_t i = 0; i < subjects.size(); ++i){
        subjects[i]->detach(this);
    }
}