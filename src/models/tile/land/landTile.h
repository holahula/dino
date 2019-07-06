#ifndef LAND_TILE_H_
#define LAND_TILE_H_

#include "../tile.h"

class Tower;

class LandTile : public Tile {
    Tower * tower;
    public:
        LandTile(int, int);
        ~LandTile();
        char getType() const override;
        bool isOccupied() const;
        void addTower(Tower *);
};

#endif
