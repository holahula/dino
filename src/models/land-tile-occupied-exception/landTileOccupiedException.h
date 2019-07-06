#ifndef LAND_TILE_OCCUPIED_EXCEPTION_H_
#define LAND_TILE_OCCUPIED_EXCEPTION_H_

#include <string>

class LandTileOccupiedException {
    std::string msg;

    public:
        LandTileOccupiedException(std::string);
        std::string what();
};

#endif