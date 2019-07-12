#include "shop.h"

using namespace std;

Shop::Shop(){}
Shop::~Shop(){}

bool Shop::buy(int money, char type) {
    if ((type == 'D' && money >= DamageTower::cost)
    || (type == 'F' && money >= FreezeTower::cost)
    || (type == 'M' && money >= MoneyTower::cost)
    ) {
        return true;
    }
    
    return false;
}

Tower* Shop::newTower(int& money, char type){
    Tower* t = nullptr;

    if(type == 'D'){
        money -= DamageTower::cost;
        t = new DamageTower();
    } else if (type == 'F'){
        money -= FreezeTower::cost;
        t = new FreezeTower();
    } else if (type == 'M'){
        money -= MoneyTower::cost;
        t = new MoneyTower();
    }

    return t;
}

void Shop::sell(int& money, Tower* tower) {
    money += 0.75*tower->getTotalValue();
}

void Shop::upgradeTower(int& money, int cost){
    money -= cost;
}