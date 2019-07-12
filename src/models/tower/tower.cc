#include "tower.h"

using namespace std;

Tower::Tower(int startingValue) : range(3), totalValue(startingValue) {}
Tower::~Tower() {}

int Tower::getRange() const {
    return range;
}

void Tower::attack() {
    notifyObservers(this);
}

int Tower::getTotalValue() const {
    return totalValue;
}
