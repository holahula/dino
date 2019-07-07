#ifndef SHOP_H_
#define SHOP_H_

#include "./../tower/tower.h"
#include "./../tower/damage/damage.h"
#include "./../tower/freeze/freeze.h"
#include "./../tower/money/money.h"
#include "../tower/tower.h"

#include <utility>

class Shop {


    public:
        Shop();
        ~Shop();

        bool buy(int, char);
        void sell(int &, Tower *);

        Tower* newTower(int&, char);

        void upgradeTower(int&, int);

        // MoneyTower* newMoneyTower(int&);
};

#endif