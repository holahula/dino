#ifndef MAP_H_
#define MAP_H_

#include "../tile/tile.h"
#include "../tile/path/pathTile.h"
#include "../enemy/enemy.h"
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

class Map {
    int width;
    int height;
    std::vector<std::vector<Tile*> > map;
    std::vector<PathTile*> path;
    std::vector<std::pair<int,int> > createPath(std::pair<int,int> from, std::pair<int,int> to);
    std::unordered_map<PathTile*, std::unordered_set<Tower*> > insideRange;
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
    public:
        Map();
        ~Map();
        std::vector<Enemy*> nextFrame();
        std::vector<Enemy*> removeDeadEnemies();
        void attachAllEnemies();
        void detachAllEnemies();
        bool isOccupied(int x, int y);
        bool isTower(int x, int y);
        bool inMap(int x, int y);
        Tower *getTower(int x, int y);
        void insertTower(Tower*, int x, int y);
        void increaseTowerRange(Tower*, int x, int y);
        void insertEnemy(Enemy*);
        int getWidth();
        int getHeight();
        friend std::ostream& operator<<(std::ostream&, Map &);

		class Iterator {
			int row, col, nr, nc;
			std::vector<std::vector<Tile*> > map;

			Iterator(int, int, int, int, std::vector<std::vector<Tile*> >);
			friend class Map;

		  public:
		  	Tile& operator*();
			Iterator& operator++();
			bool operator!=(const Iterator&);
		};

		Iterator begin();
		Iterator end();
};

#endif
