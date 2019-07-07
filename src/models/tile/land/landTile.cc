#include "landTile.h"
#include "../../tower/tower.h"
#include "../../../exceptions/land-tile-occupied-exception/landTileOccupiedException.h"
using namespace std;

LandTile::LandTile(int x, int y): Tile(x,y), tower(nullptr) {}

LandTile::~LandTile(){}

char LandTile::getType() const {
    if (isOccupied()) {
        return tower->getType().first;
    }
    return '.';
}

bool LandTile::isOccupied() const {
    return tower ? true : false;
}

void LandTile::addTower(Tower * addedTower) {
    if (isOccupied()) {
        throw LandTileOccupiedException("There is already a tower on this land tile");
    }
    tower = addedTower;
}