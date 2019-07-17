#include "basic.h"

using namespace std;

BasicEnemy::BasicEnemy(int hp): Enemy(hp) {}

char BasicEnemy::getType_() const {
    return 'B';
}

void BasicEnemy::nextState_() {}

BasicEnemy::~BasicEnemy(){}