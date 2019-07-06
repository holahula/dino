#include "./../tower.h"
#include "damage.h"

#include <utility>

using namespace std;

DamageTower::DamageTower() : Tower(), dmg(1), upCost(1) {};
DamageTower::~DamageTower(){}

int DamageTower::getUpgradeCost() const {
    return upCost;
}

std::pair<char, int> DamageTower::getType() const {
    return make_pair('D', dmg);
}

int DamageTower::getCost() const {
    return cost;
}

void DamageTower::upgrade() {
    dmg *= 2;
    upCost *= 2;
    range++;
}

