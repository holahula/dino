#include "tile.h"
#include <utility>

using namespace std;

Tile::Tile(int x, int y): x(x), y(y){}

pair<int,int> Tile::location() const {
    return make_pair(x,y);
}

bool Tile::isOccupied() const {
    return isOccupied_();
}

char Tile::getType() const {
    return getType_();
}

Tile::~Tile(){}