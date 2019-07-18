#ifndef FREEZE_H_
#define FREEZE_H_

#include "./../tower.h"

class FreezeTower : public Tower {
    private:
        int duration, upCost;
        int getCost_() const override;
        std::string getDescription_() const override;
        int getUpgradeCost_() const override;
        std::pair<char, int> getType_() const override;

        void upgrade_() override;
    public:
        static const int cost = 20;

        FreezeTower();
        virtual ~FreezeTower();
};

#endif
