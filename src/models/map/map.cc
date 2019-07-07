#include "map.h"
#include "../tile/land/landTile.h"
#include "../tile/path/pathTile.h"
#include <vector>
#include <utility>
#include <set>

using namespace std;

Map::~Map(){
    for (int i=0; i<map.size(); ++i) {
        for (int j=0; j<map[i].size(); ++j) {
            delete map[i][j];
        }
    }
}

ostream& operator<<(ostream& out, Map & currFrame) {
    for (int i=0; i<currFrame.map.size(); ++i) {
        for (int j=0; j<currFrame.map[i].size(); ++j) {
            out << currFrame.map[i][j]->getType() << " ";
        }
        out << endl;
    }
    return out;
}

void Map::insertEnemy(Enemy * newEnemy) {
    path[0]->insertEnemy(newEnemy);
}

Map::Map() {
    srand(chrono::system_clock::now().time_since_epoch().count());
    width = rand()%3 + 4;
    height = rand()%3 + 4;

    this->map = vector<vector<Tile*> >(width, vector<Tile*>(height, nullptr));
    initPath();
    initMap();
}


void Map::initPath() {
    vector<pair<int,int> > createdPath = createPath(make_pair(0, rand()%height), make_pair(width-1, rand()%height));

    path = vector<PathTile*>(createdPath.size(), nullptr);
    for (int i=createdPath.size()-1; i>=0; --i) {
        path[i] = new PathTile(createdPath[i].first, createdPath[i].second);
        if (i != createdPath.size()-1) {
            path[i]->next.push_back(path[i+1]);
            path[i+1]->prev.push_back(path[i]);
        }
        map[path[i]->location().first][path[i]->location().second] = path[i];
    }
}

void Map::initMap() {
    for (int i=0; i<map.size(); ++i) {
        for (int j=0; j<map[i].size(); ++j) {
            if (!map[i][j]) {
                map[i][j] = new LandTile(i,j);
            }
        }
    }
}

bool Map::createPathHelper (
    vector<vector<bool> >& visited,
    pair<int,int> curr,
    pair<int,int> dest,
    vector<pair<int,int> >& path) {
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
            if (next.first < 0 || next.first >= width || next.second < 0 || next.second >= height) continue;
            if (visited[next.first][next.second]) continue;

            visited[next.first][next.second] = true;
            path.push_back(next);
            if(createPathHelper(visited, next, dest, path)){
                return true;
            }
            path.pop_back();
            visited[next.first][next.second] = false;
        } while (currIdx != randomIdx);
        return false;
}

vector<pair<int,int> > Map::createPath(pair<int,int> from, pair<int,int> to) {
    vector<vector<bool> > visited = vector<vector<bool> >(width, vector<bool>(height, false));
    vector<pair<int,int> > path;
    visited[from.first][from.second] = true;
    path.push_back(from);

    createPathHelper(visited, from, to, path);
    return path;
}

vector<Enemy*> Map::nextFrame() {
    vector<Enemy*> before = path[path.size()-1]->getEnemies();
    for (int i=path.size()-1; i>=0; --i) {
        path[i]->moveEnemies();
    }
    vector<Enemy*> after = path[path.size()-1]->getEnemies();

    set<Enemy*> beforeSet = set<Enemy*>(before.begin(), before.end());
    for (int i=0; i<after.size(); ++i) {
        beforeSet.erase(after[i]);
    }
    return vector<Enemy*>(beforeSet.begin(), beforeSet.end());
}

vector<Enemy*> Map::removeDeadEnemies() {
    vector<Enemy*> deadEnemies;
    for (int i=0; i<path.size(); ++i) {
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