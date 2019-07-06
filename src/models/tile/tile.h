#ifndef TILE_H_
#define TILE_H_

#include <utility>

class Tile {
    protected:
        int x;
        int y;
    public:
        Tile(int x, int y);
        std::pair<int,int> location();
        virtual char print() = 0;
        virtual ~Tile() = 0;
};

#endif
