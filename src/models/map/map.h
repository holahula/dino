#ifndef MAP_H_
#define MAP_H_

#include "./tile/tile.h"
#include "./tile/path/pathTile.h"
#include "./tile/land/landTile.h"
#include "../enemy/enemy.h"
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <memory>

struct MapImpl {
    int width;
    int height;
    std::vector<std::vector<Tile*> > map;
    std::vector<PathTile*> path;
    std::vector<LandTile*> land;
    std::unordered_map<PathTile*, std::unordered_set<Tower*> > insideRange;
    public:
        ~MapImpl();
};

class Map {
    MapImpl* p;

    std::vector<std::pair<int,int> > createPath(std::pair<int,int> from, std::pair<int,int> to);
    bool checkSquare(std::vector<std::vector<bool> >& visited, std::pair<int,int> x, std::pair<int,int> y);
    bool isSquare(std::vector<std::vector<bool> >& visited, std::pair<int,int> curr);
    bool smallMapPathBuilder(
        std::vector<std::vector<bool> >&,
        std::pair<int,int>,
        std::pair<int,int>,
        std::vector<std::pair<int,int> >&
    );
    void bigMapPathBuilder (
        std::pair<int,int> from,
        std::pair<int,int> to,
        std::vector<std::pair<int,int> >& path
    );
    void initPath();
    void initMap();

    class Iterator {
        int col, row, nc, nr;
        Map* m;

        Iterator(int, int, int, int, Map*);
        friend class Map;

        public:
            Tile* operator*();
            Iterator& operator++();
            bool operator!=(const Iterator&);
    };

    public:
        Map();
        ~Map();

		Iterator begin();
		Iterator end();

        std::vector<Enemy*> nextFrame();
        std::vector<Enemy*> removeDeadEnemies();
        void attachAllEnemies();
        void detachAllEnemies();
        bool isOccupied(int x, int y);
        bool isTower(int x, int y);
        bool inMap(int x, int y);
        Tower *getTower(int x, int y);
        LandTile* getLandTile(int x, int y);
        PathTile* getPathTile(int x, int y);
		Tile* getTile(int x, int y);
        void insertTower(Tower*, int x, int y);
        bool sellTower(int x, int y);
        void increaseTowerRange(Tower*, int x, int y);
        void insertEnemy(Enemy*);
        int getWidth();
        int getHeight();
        friend std::ostream& operator<<(std::ostream&, Map &);
};

#endif
