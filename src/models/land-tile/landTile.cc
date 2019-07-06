#include "landTile.h"
#include "../tower/tower.h"
#include "../../exceptions/land-tile-occupied-exception/landTileOccupiedException.h"

using namespace std;


LandTile::LandTile(int x, int y): Tile(x,y), tower(nullptr) {}

LandTile::~LandTile(){}

char LandTile::print() {
    if (isOccupied()) {
        return tower->print();
    }
    return ' ';
}

bool LandTile::isOccupied() {
    return tower ? true : false;
}

void LandTile::addTower(Tower * addedTower) {
    if (isOccupied()) {
        throw LandTileOccupiedException("There is already a tower on this land tile");
    }
    tower = addedTower;
}