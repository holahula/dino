#include "./../tower.h"
#include "money.h"

#include <utility>

using namespace std;

MoneyTower::MoneyTower() : Tower(), income(5), upCost(5){};
MoneyTower::~MoneyTower() {}

int MoneyTower::getIncome() const {
    return income;
}

int MoneyTower::getUpgradeCost() const {
    return upCost;
}

// no one calls this function, just satisifies virtual void = 0
std::pair<char, int> MoneyTower::getType() const {
    return make_pair('M', income);
}

int MoneyTower::getCost() const {
    return cost;
}
void MoneyTower::upgrade() {
    income += 5;
    upCost += 5;
}

