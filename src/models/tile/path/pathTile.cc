#include "pathTile.h"
#include <vector>
#include <chrono>

using namespace std;

PathTile::PathTile(int x, int y, vector<PathTile*> next, vector<PathTile*> prev): Tile(x,y), next(next), prev(prev){};

PathTile::~PathTile(){}

bool PathTile::endOfPath() {
    return next.empty() ? true : false;
}

void PathTile::moveEnemies() {
    if (!endOfPath()) {
        srand(chrono::system_clock::now().time_since_epoch().count());
        for (int i=0; i<enemies.size(); ++i) {
            next[rand()%next.size()]->enemies.push_back(enemies[i]);
        }
    }
    enemies.clear();
}

char PathTile::print(){
    return 'P';
}

vector<Enemy *> PathTile::getEnemies() {
    return enemies;
}
