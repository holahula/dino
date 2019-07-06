#ifndef LAND_TILE_H_
#define LAND_TILE_H_

#include "../tile/tile.h"

class Tower;

class LandTile : public Tile {
    Tower * tower;
    public:
        LandTile(int, int);
        ~LandTile();
        virtual char print() override;
        bool isOccupied();
        void addTower(Tower *);
};

#endif
