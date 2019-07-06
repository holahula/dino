#include "regenerative.h"
#include <cmath>
#include <algorithm>

using namespace std;

RegenerativeEnemy::RegenerativeEnemy(int hp, int regen): Enemy(hp), maxHP(hp), regen(regen){}

void RegenerativeEnemy::heal(){
    hp = min(maxHP, hp+regen);
}

char RegenerativeEnemy::getType() const {
    return 'R';
}