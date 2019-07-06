#ifndef PATH_TILE_H_
#define PATH_TILE_H_

#include "../tile.h"
#include "../../enemy/enemy.h"
#include <vector>

class PathTile : public Tile {
    std::vector<Enemy*> enemies;
    const std::vector<PathTile*> next;
    const std::vector<PathTile*> prev;
    
    public:
        PathTile(int, int, std::vector<PathTile*>, std::vector<PathTile*>);
        ~PathTile();
        bool endOfPath() const;
        void moveEnemies();
        char getType() const override;
        std::vector<Enemy*> getEnemies() const ;
};

#endif
