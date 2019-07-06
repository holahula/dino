#include "tower.h"

using namespace std;

Tower::Tower() {}
Tower::~Tower() {}

void Tower::attack() {
    notifyObservers(this);
}

