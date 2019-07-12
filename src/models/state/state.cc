#include "./../shop/shop.h"
#include "./../map/map.h"
#include "./../enemy/basic/basic.h"
#include "state.h"

#include <vector>
#include <algorithm>

using namespace std;

State::State(): hp(100), money(10), round(1), shop(new Shop()), map(new Map()){}

State::~State() {
    for(Tower* tower : towers) delete tower;
    for(Enemy* enemy : enemies) delete enemy;
    delete shop;
    delete map;
}  

void State::displayMap(){
    cout << *map << endl;
}

void State::incrementMoney(int amount) {
    money += amount;
}

bool State::surviveDmg(int amount) {
    if(hp-amount <= 0) return false;

    hp -= amount;
    return true;
}

void State::addTower(Tower* t) {
    towers.push_back(t);
}

void State::addEnemy(Enemy* e){
    pendingEnemies.push(e);
    enemies.push_back(e);
}

void State::removeEnemy(Enemy* enemy){
    enemies.erase(remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    delete enemy;
}

/******************************
    BETWEEN ROUND FUNCTIONS
******************************/

bool State::buyTower(char type, int x, int y){
    if (!map->inMap(x, y) || map->isOccupied(x, y) || !shop->buy(money, type)) {
        return false;
    }
    
    Tower* t = shop->newTower(money, type);
    addTower(t);

    // if(type == 'D'){
    //     t = shop->newDamageTower(money);
    // }

    map->insertTower(t, x, y);

    return true;
}

bool State::upgradeTower(int x, int y){
    if (!map->inMap(x, y) || !map->isTower(x, y) || money < map->getTower(x, y)->getUpgradeCost()) {
        return false;
    }

    Tower* t = map->getTower(x, y);
    int upgradeCost = t->getUpgradeCost();

    shop->upgradeTower(money, upgradeCost);
    t->upgrade();
    map->increaseTowerRange(t, x, y);

    return true;
}

/*****************************
    DURING ROUND FUNCTIONS
*****************************/

// based on round, construct enemies -> insert them into state enemy vector
int State::constructEnemies(int round) {
    for(int i = 0; i < 10; i++){
        addEnemy(new BasicEnemy(2));
    }
    return 10;
}

int State::totalHPLost(vector<Enemy*> enemies){
    int total = 0;
    for(auto &e : enemies) {
        total += e->getHP();
    }
    return total;
}

bool State::moveEnemies(int frame, int size){
    vector<Enemy*> escaped = map->nextFrame();

    if(!surviveDmg(totalHPLost(escaped))){
        return false;
    }

    for(Enemy* enemy: escaped){
        removeEnemy(enemy);
    }

    if (!pendingEnemies.empty()){
        map->insertEnemy(pendingEnemies.top());
        pendingEnemies.pop();
    }

    return true;
}

bool State::preFrame(int frame, int size){
    if(!moveEnemies(frame, size)){ 
        return false;
    }
    map->attachAllEnemies();
    return true;
}

void State::processFrame(){
    // shoot enemies
    for(Tower* tower : towers){
        pair<int, int> type = tower->getType();
        if(type.first == 'D'){
            incrementMoney(type.second);
        }
        tower->notifyObservers(tower);
    }

    vector<Enemy*> dead =  map->removeDeadEnemies();
    for (auto enemy : dead) {
        removeEnemy(enemy);
    }
}

// prepares for the next frame, detachs all the enemies from their respective towers
void State::postFrame(){
    displayMap();
    map->detachAllEnemies();
}

void State::getMoneyTowerIncome(){
    for(auto& tower : towers){
        pair<int, int> type = tower->getType();
        if(type.first == 'M'){
            money += type.second;
        }
    }
}

void State::updateState(int hp, int round){
    if(hp <= 0) {
        // TODO: handle death, clean up everything
        cout << "dead on round " << round << endl;
    } else if(round == MAX_ROUND){
        cout << "winner winner chicken dinner" << endl;
    }

    getMoneyTowerIncome();

    cout << "You have " << hp << " HP" << endl;
    cout << "You have $" << money << endl;
    
    round++;
}

void State::startRound(){
    cout << "Round " << round << "!" << endl;

    int frame = 1;
    int size = constructEnemies(round);
    bool status;
    // round while loop 
    while(enemies.size() != 0){
        cout << "Frame: " << frame << endl;
        status = preFrame(frame, size);
        if(!status) {
            cout << "pre-frame status error" << endl;
            break;
        }
        processFrame();

        postFrame();

        frame++;
    }
    updateState(hp, round);
}

int State::getMoney() {
	return money;
}

int State::getHp() {
	return hp;
}

int State::getRound() {
	return round;
}
