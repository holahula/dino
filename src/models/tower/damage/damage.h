#ifndef DAMAGE_H_
#define DAMAGE_H_

#include "./../tower.h"

class DamageTower : public Tower {
    private:
        int dmg, upCost;

    public:
        static const int cost = 10;

        DamageTower();
        ~DamageTower();

        int getUpgradeCost() const;
        std::pair<char, int> getType() const override;
        void upgrade() override;
};

#endif
