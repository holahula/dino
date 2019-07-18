#include "./../tower.h"
#include "damage.h"

#include <utility>
#include <string>
#include <cmath>

using namespace std;

DamageTower::DamageTower() : Tower(DamageTower::cost), dmg(1), upCost(1) {};
DamageTower::~DamageTower(){}

int DamageTower::getUpgradeCost_() const {
    return upCost;
}

std::pair<char, int> DamageTower::getType_() const {
    return make_pair('D', dmg);
}

int DamageTower::getCost_() const {
    return cost;
}

string DamageTower::getDescription_() const {
    return "Attacks enemies \nwith a certain \ndamage";
}

void DamageTower::upgrade_() {
    totalValue += upCost;
    dmg *= 2;
    upCost *= 4;
    ++range;
}

