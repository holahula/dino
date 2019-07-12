#include "./../tower.h"
#include "money.h"

#include <utility>

using namespace std;

MoneyTower::MoneyTower() : Tower(MoneyTower::cost), income(5), upCost(5){};
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

void MoneyTower::upgrade() {
    totalValue += upCost;
    income += 5;
    upCost += 5;
}

