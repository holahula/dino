#include "./../tower.h"
#include "freeze.h"

#include <utility>

using namespace std;


FreezeTower::FreezeTower() : Tower(), duration(1), upCost(5){};
FreezeTower::~FreezeTower() {}

int FreezeTower::getUpgradeCost() const {
    return upCost;
}

std::pair<char, int> FreezeTower::getType() const {
    return make_pair('D', duration);
}

int FreezeTower::getCost() const {
    return cost;
}

void FreezeTower::upgrade() {
    range++; 
    duration++;
    upCost += 5;
}

