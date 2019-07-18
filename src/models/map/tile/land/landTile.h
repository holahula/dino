#ifndef LAND_TILE_H_
#define LAND_TILE_H_

#include "../tile.h"

class Tower;

class LandTile : public Tile {
    Tower * tower;
	char getType_() const override;
	bool isOccupied_() const override;
    public:
        LandTile(int, int);
        ~LandTile();
        void removeTower();
        Tower* getTower();
        void addTower(Tower *);
};

#endif
