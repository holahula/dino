#include "pathTile.h"
#include <vector>
#include <chrono>

using namespace std;

PathTile::PathTile(int x, int y): Tile(x,y), next(vector<PathTile*>()), prev(vector<PathTile*>()){};

PathTile::~PathTile(){}

bool PathTile::endOfPath() const {
    return next.empty() ? true : false;
}

void PathTile::moveEnemies() {
    srand(chrono::system_clock::now().time_since_epoch().count());
    vector<Enemy*> frozen;
    for (int i=0; i<enemies.size(); ++i) {
        if (enemies[i]->getFrozen() > 0) {
            enemies[i]->decFrozen();
            frozen.push_back(enemies[i]);
            continue;
        }
        if (!endOfPath()) {
            next[rand()%next.size()]->enemies.push_back(enemies[i]);
        }
    }
    enemies.clear();
    enemies = frozen;
}

char PathTile::getType() const {
    if (next.empty()) {
        return 'E';
    } else if (prev.empty()) {
        return 'S';
    } else {
        return 'P';
    }
}

vector<Enemy *>& PathTile::getEnemies() {
    return enemies;
}

void PathTile::insertEnemy(Enemy* newEnemy) {
    enemies.push_back(newEnemy);
}