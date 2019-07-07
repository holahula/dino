#include "pathTile.h"
#include <vector>
#include <chrono>

using namespace std;

PathTile::PathTile(int x, int y): Tile(x,y), next(vector<PathTile*>()), prev(vector<PathTile*>()){};

PathTile::~PathTile(){}

bool PathTile::endOfPath() const {
    return next.empty() ? true : false;
}

vector<Enemy*> PathTile::moveEnemies() {
    srand(chrono::system_clock::now().time_since_epoch().count());
    vector<Enemy*> frozen;
    vector<Enemy*> killed;

    for (int i=0; i<enemies.size(); ++i) {
        if (enemies[i]->getHP() <= 0) {
            killed.push_back(enemies[i]);
        }
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
    return killed;
}

char PathTile::getType() const {
    return 'P';
}

vector<Enemy *> PathTile::getEnemies() const {
    return enemies;
}

void PathTile::insertEnemy(Enemy* newEnemy) {
    enemies.push_back(newEnemy);
}