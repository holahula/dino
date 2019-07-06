#ifndef MAP_H_
#define MAP_H_

#include "../tile/tile.h"
#include "../tile/path/pathTile.h"
#include "../enemy/enemy.h"
#include <vector>
#include <utility>
#include <iostream>

class Map {
    std::vector<std::vector<Tile*> > map;
    std::vector<PathTile*> path;
    std::vector<std::pair<int,int> > getPath(std::pair<int,int> from, std::pair<int,int> to);
    public:
        Map();
        ~Map();

        // returns vector of dead enemies and completed enemies
        std::pair<std::vector<Enemy*>, std::vector<Enemy*> > nextFrame();
        friend std::ostream& operator<<(std::ostream&, Map &);
};

#endif
