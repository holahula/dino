#include "map.h"
#include "./tile/land/landTile.h"
#include "./tile/path/pathTile.h"
#include "../../exceptions/land-tile-occupied-exception/landTileOccupiedException.h"
#include "../../exceptions/no-tower-exception/noTowerException.h"
#include <vector>
#include <utility>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <chrono>
#include <algorithm>

using namespace std;


MapImpl::~MapImpl() {
    for (size_t i=0; i<map.size(); ++i) {
        for (size_t j=0; j<map[i].size(); ++j) {
            delete map[i][j];
        }
    }
}

Map::Map(): p(new MapImpl()) {
    srand(chrono::system_clock::now().time_since_epoch().count());
    p->width = rand()%4 + 8;
    p->height = rand()%4 + 8;

    p->map = vector<vector<Tile*> >(p->width, vector<Tile*>(p->height, nullptr));
    initPath();
    initMap();
}

Map::~Map(){
    delete p;
}

int Map::getWidth() {
    return p->width;
}

int Map::getHeight() {
    return p->height;
}

int square(int x) {
    return x*x;
}

int distance(pair<int,int> lhs, pair<int,int> rhs) {
    return ceil(sqrt(square(lhs.first - rhs.first) + square(lhs.second - rhs.second)));
}

ostream& operator<<(ostream& out, Map & currFrame) {
    for (int j=currFrame.getHeight()-1; j>=0; --j) {
        out << j << " ";
        for (int i=0; i<currFrame.getWidth(); ++i) {
            out << currFrame.p->map[i][j]->getType() << " ";
        }
        out << endl;
    }
    out << "  ";
    for (int i=0; i<currFrame.getWidth(); ++i) {
        out << i << " ";
    }
    out << endl;

    for (size_t i=0; i<currFrame.p->path.size(); ++i) {
        if (currFrame.p->path[i]->getEnemies().empty()) {
            continue;
        }
        out << "Path Tile: (" << currFrame.p->path[i]->location().first << ", " << currFrame.p->path[i]->location().second;
        out << "), has balloons: " << endl;
        out << *currFrame.p->path[i];
    }
    return out;
}

void Map::insertEnemy(Enemy* newEnemy) {
    p->path[0]->insertEnemy(newEnemy);
}


void Map::initPath() {
    vector<pair<int,int> > createdPath = createPath(make_pair(0, rand()%p->height), make_pair(p->width-1, rand()%p->height));

    p->path = vector<PathTile*>(createdPath.size(), nullptr);
    for (int i=(int)createdPath.size()-1; i>=0; --i) {
        p->path[i] = new PathTile(createdPath[i].first, createdPath[i].second);
        if (i != (int)createdPath.size()-1) {
            p->path[i]->next.push_back(p->path[i+1]);
            p->path[i+1]->prev.push_back(p->path[i]);
        }
        p->map[p->path[i]->location().first][p->path[i]->location().second] = p->path[i];
    }
}

void Map::initMap() {
    for (size_t i=0; i<p->map.size(); ++i) {
        for (size_t j=0; j<p->map[i].size(); ++j) {
            if (!p->map[i][j]) {
                LandTile * curr = new LandTile(i,j);
                p->map[i][j] = curr;
                p->land.push_back(curr);
            }
        }
    }
}

bool Map::checkSquare(vector<vector<bool> >& visited, pair<int,int> x, pair<int,int> y) {
    if (x.first < 0 || x.second >= p->width) return false;
    if (y.first < 0 || y.second >= p->height) return false;
    int count = 0;
    for (int i=x.first; i<=x.second; ++i) {
        for (int j=y.first; j<=y.second; ++j) {
            if (visited[i][j]) {
                ++count;
            }
        }
    }
    return count >= 4;
}

bool Map::isSquare(vector<vector<bool> >& visited, pair<int,int> curr) {
    if (checkSquare(visited, make_pair(curr.first-1, curr.first), make_pair(curr.second, curr.second+1))
        || checkSquare(visited, make_pair(curr.first, curr.first+1), make_pair(curr.second, curr.second+1))
        || checkSquare(visited, make_pair(curr.first-1, curr.first), make_pair(curr.second-1, curr.second))
        || checkSquare(visited, make_pair(curr.first, curr.first+1), make_pair(curr.second-1, curr.second))) {
        return true;
    }
    return false;
}

bool Map::smallMapPathBuilder (
    vector<vector<bool> >& visited,
    pair<int,int> curr,
    pair<int,int> dest,
    vector<pair<int,int> >& path) {
        if (isSquare(visited, curr)) {
            return false;
        }
        if (curr.first == dest.first && curr.second == dest.second) {
            return true;
        }

        // create directions
        vector<pair<int,int> > dir;
        dir.push_back(make_pair(0,1));
        dir.push_back(make_pair(1,0));
        dir.push_back(make_pair(0,-1));
        dir.push_back(make_pair(-1,0));

        srand(chrono::system_clock::now().time_since_epoch().count());
        int randomIdx = rand()%dir.size();
        int currIdx = randomIdx;
        do {
            pair<int,int> next = make_pair(curr.first+dir[currIdx].first, curr.second+dir[currIdx].second);
            currIdx = (currIdx + 1) % dir.size();
            if (next.first < 0 || next.first >= p->width || next.second < 0 || next.second >= p->height){
                continue;
            };
            if (visited[next.first][next.second]){
                continue;
            };

            visited[next.first][next.second] = true;
            path.push_back(next);
            if(smallMapPathBuilder(visited, next, dest, path)){
                return true;
            }
            path.pop_back();
            visited[next.first][next.second] = false;
        } while (currIdx != randomIdx);
        return false;
}

void Map::bigMapPathBuilder (
    pair<int,int> from,
    pair<int,int> to,
    vector<pair<int,int> >& path) {
        // create directions
        vector<pair<int,int> > dir;
        dir.push_back(make_pair(0,1));
        dir.push_back(make_pair(1,0));
        dir.push_back(make_pair(0,-1));

        pair<int,int> curr = from;
        while (curr.first != to.first || curr.second != to.second) {
            pair<int,int> minLoc = curr;
            int minDist = square(minLoc.first-to.first) + square(minLoc.second-to.second);

            for (size_t i=0; i<dir.size(); ++i) {
                pair<int, int> nextLoc = make_pair(curr.first + dir[i].first, curr.second + dir[i].second);
                int nextDist = square(nextLoc.first-to.first) + square(nextLoc.second-to.second);
                if (minDist > nextDist) {
                    minDist = nextDist;
                    minLoc = nextLoc;
                }
            }
            curr = minLoc;
            path.push_back(curr);
        }
}

vector<pair<int,int> > Map::createPath(pair<int,int> from, pair<int,int> to) {
    vector<pair<int,int> > path;
    path.push_back(from);

    if (p->width >= 10 || p->height >= 10) {
        bigMapPathBuilder(from, to, path);
    } else {
        vector<vector<bool> > visited = vector<vector<bool> >(p->width, vector<bool>(p->height, false));
        visited[from.first][from.second] = true;
        smallMapPathBuilder(visited, from, to, path);
    }
    return path;
}

// returns the enemies that have left the map
vector<Enemy*> Map::nextFrame() {
    vector<Enemy*> before = p->path[p->path.size()-1]->getEnemies();
    for (int i=(int)p->path.size()-1; i>=0; --i) {
        p->path[i]->moveEnemies();
    }
    vector<Enemy*> after = p->path[p->path.size()-1]->getEnemies();

    set<Enemy*> beforeSet = set<Enemy*>(before.begin(), before.end());
    for (size_t i=0; i<after.size(); ++i) {
        beforeSet.erase(after[i]);
    }
    return vector<Enemy*>(beforeSet.begin(), beforeSet.end());
}

vector<Enemy*> Map::removeDeadEnemies() {
    vector<Enemy*> deadEnemies;
    for (size_t i=0; i<p->path.size(); ++i) {
        vector<Enemy*>& currEnemies = p->path[i]->getEnemies();
        for (Enemy* enemy : currEnemies) {
            if (enemy->getHP() <= 0) {
                deadEnemies.push_back(enemy);
                currEnemies.erase(std::remove(currEnemies.begin(), currEnemies.end(), enemy), currEnemies.end());
            }
        }
    }
    return deadEnemies;
}

bool isPathTile(char c) {
    return c == 'P' || c == 'E' || c == 'S';
}

bool Map::inMap(int x, int y) {
    return x >=0 && x<p->width && y>=0 && y<p->height;
}

bool Map::isOccupied(int x, int y) {
    return p->map[x][y]->isOccupied();
}

bool Map::isTower(int x, int y) {
    return !isPathTile(p->map[x][y]->getType()) && p->map[x][y]->isOccupied();
}

Tower* Map::getTower(int x, int y) {
    if (!isTower(x,y)) {
        throw NoTowerException("There is no tower at this location");
    }
    for (size_t i=0; i<p->land.size(); ++i) {
        if (p->land[i]->location().first == x && p->land[i]->location().second == y) {
            return p->land[i]->getTower();
        }
    }
    return nullptr;
}

LandTile* Map::getLandTile(int x, int y) {
    for (size_t i=0; i<p->land.size(); ++i) {
        if (p->land[i]->location().first == x && p->land[i]->location().second == y) {
            return p->land[i];
        }
    }
    return nullptr;
}

void Map::insertTower(Tower* tower, int x, int y) {
    // can throw a LandTileOccupiedException
    if (isOccupied(x,y)) {
        throw LandTileOccupiedException("Cannot place a tower here");
    }

    getLandTile(x,y)->addTower(tower);
    for (size_t i=0; i<p->path.size(); ++i) {
        int dist = distance(p->path[i]->location(), make_pair(x,y));
        if (dist <= tower->getRange()) {
            p->insideRange[p->path[i]].insert(tower);
        }
    }
}

void Map::increaseTowerRange(Tower* tower, int x, int y) {
    for (size_t i=0; i<p->path.size(); ++i) {
        if (!p->insideRange[p->path[i]].count(tower)) {
            int dist = distance(p->path[i]->location(), make_pair(x,y));
            if (dist <= tower->getRange()) {
                p->insideRange[p->path[i]].insert(tower);
            }
        }
    }
}

bool Map::sellTower(int x, int y) {
    if (!isTower(x,y)) {
        return false;
    }
    Tower* tower = getTower(x,y);
    for (size_t i=0; i<p->path.size(); ++i) {
        unordered_set<Tower*>& currTowers = p->insideRange[p->path[i]];
        if (currTowers.count(tower)) {
            currTowers.erase(tower);
        }
    }
    getLandTile(x,y)->removeTower();
    return true;
}

void Map::detachAllEnemies() {
    for (size_t i=0; i<p->path.size(); ++i) {
        for (Tower* tower : p->insideRange[p->path[i]]) {
            tower->detachAll();
        }
    }
}

void Map::attachAllEnemies() {
    for (int i=(int)p->path.size()-1; i>=0; --i) {
        for (Tower* tower: p->insideRange[p->path[i]]) {
            for (Enemy* enemy : p->path[i]->getEnemies()) {
                if (enemy->isTargetable()) {
                    tower->attach(enemy);
                }
            }
        }
    }
}