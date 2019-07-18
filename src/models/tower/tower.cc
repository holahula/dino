#include "tower.h"

using namespace std;

Tower::Tower(int startingValue) : range(3), totalValue(startingValue) {}
Tower::~Tower() {}

pair<char, int> Tower::getType() const {
	return getType_();
}

int Tower::getCost() const {
	return getCost_();
}

string Tower::getDescription() const {
	return getDescription_();
}

int Tower::getUpgradeCost() const {
	return getUpgradeCost_();
}

void Tower::upgrade() {
	upgrade_();
}

int Tower::getRange() const {
    return range;
}

void Tower::attack() {
    notifyObservers(this);
}

int Tower::getTotalValue() const {
    return totalValue;
}
