#ifndef FREEZE_H_
#define FREEZE_H_

#include "./../tower.h"

class FreezeTower : public Tower {
    private:
        int duration, upCost;
    public:
        static const int cost = 20;

        FreezeTower();
        ~FreezeTower();

        int getUpgradeCost() const;
        std::pair<char, int> getType() const override;
        void upgrade() override;

};

#endif
