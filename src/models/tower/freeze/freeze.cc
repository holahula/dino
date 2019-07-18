#include "./../tower.h"
#include "freeze.h"

#include <utility>
#include <string>

using namespace std;


FreezeTower::FreezeTower() : Tower(FreezeTower::cost), duration(2), upCost(1){};
FreezeTower::~FreezeTower() {}

int FreezeTower::getUpgradeCost_() const {
    return upCost;
}

std::pair<char, int> FreezeTower::getType_() const {
    return make_pair('F', duration);
}

int FreezeTower::getCost_() const {
    return cost;
}

string FreezeTower::getDescription_() const {
	return "Slows down \nenemies for a \ncertain duration \nof time";
}

void FreezeTower::upgrade_() {
    totalValue += upCost;
    ++range;
    ++duration;
    upCost = (duration-1)*(duration-1);
}