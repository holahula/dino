#ifndef SHOP_H_
#define SHOP_H_

#include <utility>
#include <vector>

#include "../tower/tower.h"

class Shop {
    bool buy(int, Tower *);
    void sell(int &, Tower *);

    public:

        Shop();
        ~Shop();


};

#endif