#include "regenerative.h"
#include <cmath>
#include <algorithm>

using namespace std;

RegenerativeEnemy::RegenerativeEnemy(int hp, int regen): Enemy(hp), maxHP(hp), regen(regen){}

void RegenerativeEnemy::heal(){
    hp = min(maxHP, hp+regen);
}

void RegenerativeEnemy::nextState_() {
    heal();
}

char RegenerativeEnemy::getType_() const {
    return 'R';
}

RegenerativeEnemy::~RegenerativeEnemy(){}