#ifndef NO_TOWER_EXCEPTION_H_
#define NO_TOWER_EXCEPTION_H_

#include <string>

class NoTowerException {
    std::string msg;

    public:
        NoTowerException(std::string);
        std::string what();
};

#endif