#ifndef TILE_H_
#define TILE_H_

#include <utility>

class Tile {
    private:
        virtual bool isOccupied_() const = 0;
        virtual char getType_() const = 0;
    protected:
        int x;
        int y;
    public:
        Tile(int x, int y);
        std::pair<int,int> location() const;
        bool isOccupied() const;
        char getType() const;
        virtual ~Tile() = 0;
};

#endif
