#include "enemy.h"
#include "./../tower/tower.h"

using namespace std;

Enemy::Enemy(int hp) : hp(hp), isSlowed(false) {}

void Enemy::notify(Tower * tower){
    char type = tower->getType();

    if(type == 'D') hp -= tower->dmg;
    else isSlowed = true;
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