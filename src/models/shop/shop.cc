#include "shop.h"

using namespace std;

Inventory::~Inventory(){
    delete freezeTower;
    delete damageTower;
    delete moneyTower;
}

Shop::Shop(): inv(new Inventory()){
    inv->freezeTower = new FreezeTower();
    inv->damageTower = new DamageTower();
    inv->moneyTower = new MoneyTower();
}

Shop::~Shop(){
    delete inv;
}

FreezeTower* Shop::getFreezeTower(){
    return inv->freezeTower;
}

DamageTower* Shop::getDamageTower(){
    return inv->damageTower;
}

MoneyTower* Shop::getMoneyTower(){
    return inv->moneyTower;
}

bool Shop::buy(long long money, char type) {
    if ((type == 'D' && money >= DamageTower::cost)
    || (type == 'F' && money >= FreezeTower::cost)
    || (type == 'M' && money >= MoneyTower::cost)
    ) {
        return true;
    }
    
    return false;
}

Tower* Shop::newTower(long long& money, char type){
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

void Shop::sell(long long& money, Tower* tower) {
    money += 0.75*tower->getTotalValue();
}

void Shop::upgradeTower(long long& money, int cost){
    money -= cost;
}