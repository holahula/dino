#include "./../tower/tower.h"
#include "shop.h"

using namespace std;

Shop::Shop(){}
Shop::~Shop(){}

bool Shop::buy(int money, Tower* tower) {
    if(money >= tower->getCost()) return true;
    return false;
}

void Shop::sell(int& money, Tower* tower) {
    money += tower->getCost();
}