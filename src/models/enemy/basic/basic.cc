#include "basic.h"

using namespace std;

BasicEnemy::BasicEnemy(int hp): Enemy(hp) {}

char BasicEnemy::getType() const {
    return 'B';
}

BasicEnemy::~BasicEnemy(){}