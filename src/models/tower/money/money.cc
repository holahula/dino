#include "./../tower.h"
#include "money.h"

#include <utility>
#include <string>

using namespace std;

MoneyTower::MoneyTower() : Tower(MoneyTower::cost), income(10), upCost(10){};
MoneyTower::~MoneyTower() {}

int MoneyTower::getUpgradeCost() const {
    return upCost;
}

std::pair<char, int> MoneyTower::getType() const {
    return make_pair('M', income);
}

int MoneyTower::getCost() const {
    return cost;
}

string MoneyTower::getDescription() const {
	return "Generates money \nwith a certain \nincome";
}

void MoneyTower::upgrade() {
    totalValue += upCost;
    income *= 4;
    upCost *= 2;
}

