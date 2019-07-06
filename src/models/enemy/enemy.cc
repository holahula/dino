#include "enemy.h"
#include "./../tower/tower.h"

using namespace std;

Enemy::Enemy(int hp) : hp(hp), isSlowed(false) {}

void Enemy::notify(Tower * tower){
    pair<char, int> type = tower->getType();

    if(type.first == 'D') hp -= type.second;
    else if (type.first == 'F') isSlowed = type.second;
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