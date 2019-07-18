#include "./../shop/shop.h"
#include "./../map/map.h"
#include "./../enemy/basic/basic.h"
#include "./../spawner/spawner.h"

#include "state.h"

#include <vector>
#include <algorithm>

using namespace std;

StateImpl::StateImpl(int hp, bool adaptive, bool customPath) : money(10), hp(hp), round(1), shop(new Shop()), map(new Map(customPath)), spawner(new Spawner(adaptive)){}

StateImpl::~StateImpl() {
    for(Tower* tower : towers) delete tower;
    for(Enemy* enemy : enemies) delete enemy;
    delete shop;
    delete map;
    delete spawner;
}  

State::State(bool adaptive, bool map) : p(new StateImpl(MAX_HP, adaptive, map)){}

State::~State(){
    delete p;
}

void State::displayMap(){
    cout << *p->map << endl;
}

void State::incrementMoney(int amount) {
    p->money += amount;
}

bool State::surviveDmg(int damage) {
    p->hp -= damage;

    if(p->hp <= 0) return false;

    return true;
}

void State::addTower(Tower* t) {
    p->towers.push_back(t);
}

void State::addEnemy(Enemy* e){
    p->pendingEnemies.push(e);
    p->enemies.push_back(e);
}

void State::removeEnemy(Enemy* enemy){
    p->enemies.erase(remove(p->enemies.begin(), p->enemies.end(), enemy), p->enemies.end());
    delete enemy;
}


void State::removeTower(Tower* tower){
    p->towers.erase(remove(p->towers.begin(), p->towers.end(), tower), p->towers.end());
    delete tower;
}

/******************************
    BETWEEN ROUND FUNCTIONS
******************************/

bool State::buyTower(char type, int x, int y){
    if (!p->map->inMap(x, y) || p->map->isOccupied(x, y) || !p->shop->buy(p->money, type)) {
        return false;
    }
    
    Tower* t = p->shop->newTower(p->money, type);
    addTower(t);

    p->map->insertTower(t, x, y);

    return true;
}

bool State::upgradeTower(int x, int y){
    if (!p->map->inMap(x, y) || !p->map->isTower(x, y) || p->money < p->map->getTower(x, y)->getUpgradeCost()) {
        return false;
    }

    Tower* t = p->map->getTower(x, y);

    p->shop->upgradeTower(p->money, t->getUpgradeCost());
    t->upgrade();
    p->map->increaseTowerRange(t, x, y);

    return true;
}

bool State::sellTower(int x, int y) {
    if (!p->map->inMap(x,y) || !p->map->isTower(x,y)) {
        return false;
    }
    Tower* t = p->map->getTower(x,y);
    p->shop->sell(p->money, t);
    p->map->sellTower(x, y);
    removeTower(t);
    return true;
}
/*****************************
    DURING ROUND FUNCTIONS
*****************************/

int State::constructEnemies() {
    vector<Enemy*> generatedEnemies = p->spawner->generateEnemies(p->round);
    for(auto e : generatedEnemies){
        addEnemy(e);
    }
    return generatedEnemies.size();
}

int State::totalHP(vector<Enemy*> enemies){
    int total = 0;
    for(auto &e : enemies) {
        total += e->getHP();
    }
    return total;
}

bool State::moveEnemies(int frame, int size){
    vector<Enemy*> escaped = p->map->nextFrame();

    if(!surviveDmg(totalHP(escaped))){
        return false;
    }
    // changed from Enemy* to auto
    for(auto enemy: escaped){
        removeEnemy(enemy);
    }

    if (!p->pendingEnemies.empty()){
        p->map->insertEnemy(p->pendingEnemies.top());
        p->pendingEnemies.pop();
    }

    return true;
}

bool State::preFrame(int frame, int size){
    if(!moveEnemies(frame, size)){ 
        return false;
    }
    p->map->attachAllEnemies();
    return true;
}

void State::processFrame(){
    // shoot enemies
    for(Tower* tower : p->towers){
        incrementMoney(tower->notifyObservers(tower));
    }

    vector<Enemy*> dead =  p->map->removeDeadEnemies();
    for (auto enemy : dead) {
        removeEnemy(enemy);
    }
}

// prepares for the next frame, detachs all the enemies from their respective towers
void State::postFrame(){
    displayMap();
    p->map->detachAllEnemies();
}

void State::getMoneyTowerIncome(){
    for(Tower* tower : p->towers){
        pair<int, int> type = tower->getType();
        if(type.first == 'M'){
            p->money += type.second;
        }
    }
}

void State::getRoundIncome(){
    getMoneyTowerIncome();
    p->money += p->spawner->getBonusGold();
}

void State::updateState(int hp, int hpLost, double remainingEnemyHP){
    if(hp <= 0) {
        cout << "dead on round " << p->round << endl;
        return;
    } else if(p->round == State::MAX_ROUND){
        cout << "winner winner chicken dinner" << endl;
        return;
    }

    // if(hpLost == 0 && round % 5 == 0){
    if(p->round % 5 == 0){
        int max = MAX_HP;
        cout << "You gained 5 HP for killing the boss level!" << endl;
        p->hp = min(max, p->hp+5);
    }

    getRoundIncome();
    // spawner interactions
    p->spawner->updateState(p->round, hpLost, remainingEnemyHP);


    cout << "You have " << p->hp << " HP" << endl;
    cout << "You have $" << p->money << endl;
    
    p->round++;
}

void State::startRound(){
    cout << "Round " << p->round << ":" << endl;

    int frame = 1;
    bool status;
    int size = constructEnemies();
    int hpStartRound = p->hp;
    int totalEnemyHP = totalHP(p->enemies);
    // round while loop 
    while(p->enemies.size() != 0){
        cout << "Frame: " << frame << endl;
        status = preFrame(frame, size);
        if(!status) break;

        processFrame();
        postFrame();

        frame++;
    }

    updateState(p->hp, hpStartRound - p->hp, (double)(hpStartRound - p->hp)/(double)totalEnemyHP);
}

/*****************************
    GETTERS AND SETTERS
*****************************/

int State::getHp() {
    return p->hp;
}

int State::setHp(int hp) {
    return p->hp = hp;
}

long long State::getMoney() {
    return p->money;
}

int State::getRound() {
    return p->round;
}

int State::setRound(int round) {
    return p->round = round;
}

Map* State::getMap() {
    return p->map;
}

Tower* State::getTower(int x, int y) {
    return p->map->getTower(x, y);
}

PathTile* State::getPathTile(int x, int y) {
    return p->map->getPathTile(x, y);
}

Tower* State::getTowerFromShop(char tower) {
    switch(tower) {
        case 'D':
            return p->shop->getDamageTower();
        case 'F':
            return p->shop->getFreezeTower();
        default:
            return p->shop->getMoneyTower();
    }
}

Spawner* State::getSpawner() {
    return p->spawner;
}

vector<Enemy *> State::getEnemies() {
    return p->enemies;
}
