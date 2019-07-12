#ifndef TILE_H_
#define TILE_H_

#include <utility>

class Tile {
    protected:
        int x;
        int y;
    public:
        Tile(int x, int y);
        std::pair<int,int> location() const;
        virtual bool isOccupied() const = 0;
        virtual char getType() const = 0;
        virtual ~Tile() = 0;
};

#endif
