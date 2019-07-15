#include "./../tower.h"
#include "freeze.h"

#include <utility>
#include <string>

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

string FreezeTower::getDescription() const {
	return "Slows down \nenemies for a \ncertain duration \nof time";
}

void FreezeTower::upgrade() {
    totalValue += upCost;
    range++; 
    duration++;
    upCost += 5;
}