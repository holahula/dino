#ifndef LAND_TILE_H_
#define LAND_TILE_H_

#include "../tile/tile.h"
#include <string>

class Tower;

class LandTile : public Tile {
    Tower * tower;
    public:
        LandTile(int, int);
        ~LandTile();
        void addTower(Tower *);
        bool isOccupied();
};

#endif
