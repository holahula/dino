#include "tile.h"
#include <utility>

Tile::Tile(int x, int y): x(x), y(y){}

pair<int,int> Tile::location(){
    return make_pair(x,y);
}

Tile::~Tile(){}