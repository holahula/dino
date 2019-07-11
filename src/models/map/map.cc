#include "map.h"
#include "../tile/land/landTile.h"
#include "../tile/path/pathTile.h"
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

Map::~Map(){
    for (size_t i=0; i<map.size(); ++i) {
        for (size_t j=0; j<map[i].size(); ++j) {
            delete map[i][j];
        }
    }
}

int Map::getWidth() {
    return width;
}

int Map::getHeight() {
    return height;
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
            out << currFrame.map[i][j]->getType() << " ";
        }
        out << endl;
    }
    out << "  ";
    for (int i=0; i<currFrame.getWidth(); ++i) {
        out << i << " ";
    }
    out << endl;

    for (size_t i=0; i<currFrame.path.size(); ++i) {
        if (currFrame.path[i]->getEnemies().empty()) {
            continue;
        }
        out << "Path Tile: (" << currFrame.path[i]->location().first << ", " << currFrame.path[i]->location().second;
        out << "), has balloons: " << endl;
        out << *currFrame.path[i];
    }
    return out;
}

void Map::insertEnemy(Enemy* newEnemy) {
    path[0]->insertEnemy(newEnemy);
}

Map::Map() {
    srand(chrono::system_clock::now().time_since_epoch().count());
    width = rand()%4 + 8;
    height = rand()%4 + 8;

    this->map = vector<vector<Tile*> >(width, vector<Tile*>(height, nullptr));
    initPath();
    initMap();
}


void Map::initPath() {
    vector<pair<int,int> > createdPath = createPath(make_pair(0, rand()%height), make_pair(width-1, rand()%height));

    path = vector<PathTile*>(createdPath.size(), nullptr);
    for (int i=(int)createdPath.size()-1; i>=0; --i) {
        path[i] = new PathTile(createdPath[i].first, createdPath[i].second);
        if (i != (int)createdPath.size()-1) {
            path[i]->next.push_back(path[i+1]);
            path[i+1]->prev.push_back(path[i]);
        }
        map[path[i]->location().first][path[i]->location().second] = path[i];
    }
}

void Map::initMap() {
    for (size_t i=0; i<map.size(); ++i) {
        for (size_t j=0; j<map[i].size(); ++j) {
            if (!map[i][j]) {
                map[i][j] = new LandTile(i,j);
            }
        }
    }
}

bool Map::checkSquare(vector<vector<bool> >& visited, pair<int,int> x, pair<int,int> y) {
    if (x.first < 0 || x.second >= width) return false;
    if (y.first < 0 || y.second >= height) return false;
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
            if (next.first < 0 || next.first >= width || next.second < 0 || next.second >= height){
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
        dir.push_back(make_pair(-1,0));

        pair<int,int> curr = from;
        while (curr.first != to.first || curr.second != to.second) {
            pair<int,int> minLoc = make_pair(curr.first + dir[0].first, curr.second + dir[0].second);
            int minDist = square(minLoc.first-to.first) + square(minLoc.second-to.second);

            for (size_t i=1; i<dir.size(); ++i) {
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

    if (width > 10 || height > 10) {
        bigMapPathBuilder(from, to, path);
    } else {
        vector<vector<bool> > visited = vector<vector<bool> >(width, vector<bool>(height, false));
        visited[from.first][from.second] = true;
        smallMapPathBuilder(visited, from, to, path);
    }
    return path;
}

// returns the enemies that have left the map
vector<Enemy*> Map::nextFrame() {
    vector<Enemy*> before = path[path.size()-1]->getEnemies();
    for (int i=(int)path.size()-1; i>=0; --i) {
        path[i]->moveEnemies();
    }
    vector<Enemy*> after = path[path.size()-1]->getEnemies();

    set<Enemy*> beforeSet = set<Enemy*>(before.begin(), before.end());
    for (size_t i=0; i<after.size(); ++i) {
        beforeSet.erase(after[i]);
    }
    return vector<Enemy*>(beforeSet.begin(), beforeSet.end());
}

vector<Enemy*> Map::removeDeadEnemies() {
    vector<Enemy*> deadEnemies;
    for (size_t i=0; i<path.size(); ++i) {
        vector<Enemy*>& currEnemies = path[i]->getEnemies();
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
    return x >=0 && x<width && y>=0 && y<height;
}

bool Map::isOccupied(int x, int y) {
    return isPathTile(map[x][y]->getType()) || ((LandTile*) map[x][y])->isOccupied();
}

bool Map::isTower(int x, int y) {
    return !isPathTile(map[x][y]->getType()) && ((LandTile*) map[x][y])->isOccupied();
}

Tower* Map::getTower(int x, int y) {
    if (!isTower(x,y)) {
        throw NoTowerException("There is no tower at this location");
    }
    return ((LandTile*) map[x][y])->getTower();
}

void Map::insertTower(Tower* tower, int x, int y) {
    // can throw a LandTileOccupiedException
    if (isOccupied(x,y)) {
        throw LandTileOccupiedException("Cannot place a tower here");
    }

    ((LandTile*) map[x][y])->addTower(tower);
    for (size_t i=0; i<path.size(); ++i) {
        int dist = distance(path[i]->location(), make_pair(x,y));
        if (dist <= tower->getRange()) {
            insideRange[path[i]].insert(tower);
        }
    }
}

void Map::increaseTowerRange(Tower* tower, int x, int y) {
    for (size_t i=0; i<path.size(); ++i) {
        if (!insideRange[path[i]].count(tower)) {
            int dist = distance(path[i]->location(), make_pair(x,y));
            if (dist <= tower->getRange()) {
                insideRange[path[i]].insert(tower);
            }
        }
    }
}

// inefficient -> may call detach on the same tower multiple times
void Map::detachAllEnemies() {
    for (size_t i=0; i<path.size(); ++i) {
        for (Tower* tower : insideRange[path[i]]) {
            tower->detachAll();
        }
    }
}

void Map::attachAllEnemies() {
    for (int i=(int)path.size()-1; i>=0; --i) {
        for (Tower* tower: insideRange[path[i]]) {
            for (Enemy* enemy : path[i]->getEnemies()) {
                if (enemy->isTargetable()) {
                    tower->attach(enemy);
                }
            }
        }
    }
}
