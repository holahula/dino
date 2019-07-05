#ifndef TILE_H_
#define TILE_H_

#include <utility>
using namespace std;

class Tile {
    int x;
    int y;
    public:
        Tile(int x, int y);
        pair<int,int> location();
        virtual ~Tile() = 0;
};

#endif
