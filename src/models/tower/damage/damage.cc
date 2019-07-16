#include "./../tower.h"
#include "damage.h"

#include <utility>
#include <string>

using namespace std;

DamageTower::DamageTower() : Tower(DamageTower::cost), dmg(1), upCost(1) {};
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

string DamageTower::getDescription() const {
	return "Attacks enemies \nwith a certain \ndamage";
}

void DamageTower::upgrade() {
    totalValue += upCost;
    dmg += 1;
    upCost = dmg*dmg;
    ++range;
}

