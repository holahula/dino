#ifndef PATH_TILE_H_
#define PATH_TILE_H_

#include "../tile.h"
#include "../../../enemy/enemy.h"
#include <vector>
#include <iostream>

class PathTile : public Tile {
    std::vector<Enemy*> enemies;
    char getType_() const override;
    bool isOccupied_() const override;

    public:
        std::vector<PathTile*> next;
        std::vector<PathTile*> prev;

        PathTile(int, int);
        ~PathTile();
        bool endOfPath() const;
        void moveEnemies();
        void insertEnemy(Enemy *);
        std::vector<Enemy*> & getEnemies();
        friend std::ostream& operator<<(std::ostream&, PathTile&);
};

#endif
