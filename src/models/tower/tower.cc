#include "tower.h"

using namespace std;

Tower::Tower() : range(3) {}
Tower::~Tower() {}

int Tower::getRange() const {
    return range;
}

void Tower::attack() {
    notifyObservers(this);
}


