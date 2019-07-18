#ifndef MONEY_H_
#define MONEY_H_

#include "./../tower.h"

class MoneyTower : public Tower {
    private:
        int income, upCost;
        int getCost_() const override;
        std::string getDescription_() const override;
        int getUpgradeCost_() const override;
        std::pair<char, int> getType_() const override;
        
        void upgrade_() override;

    public:
        static const int cost = 30;

        MoneyTower();
        virtual ~MoneyTower();
};

#endif
