#include "landTile.h"
#include "../land-tile-occupied-exception/landTileOccupiedException.h"
using namespace std;

class Tower;

LandTile::LandTile(int x, int y): Tile(x,y), tower(nullptr) {}

LandTile::~LandTile(){}

bool LandTile::isOccupied() {
    return tower ? true : false;
}

void LandTile::addTower(Tower * addedTower) {
    if (isOccupied()) {
        throw LandTileOccupiedException("There is already a tower on this land tile");
    }
    tower = addedTower;
}