#include "tower.h"

using namespace std;

Tower::Tower() : range(3) {}
Tower::~Tower() {}

void Tower::attack() {
    notifyObservers(this);
}

