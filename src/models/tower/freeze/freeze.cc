#include "./../tower.h"
#include "freeze.h"

#include <utility>

using namespace std;


FreezeTower::FreezeTower() : Tower(FreezeTower::cost), duration(2), upCost(5){};
FreezeTower::~FreezeTower() {}

int FreezeTower::getUpgradeCost() const {
    return upCost;
}

std::pair<char, int> FreezeTower::getType() const {
    return make_pair('F', duration);
}

int FreezeTower::getCost() const {
    return cost;
}

void FreezeTower::upgrade() {
    totalValue += upCost;
    range++; 
    duration++;
    upCost += 5;
}