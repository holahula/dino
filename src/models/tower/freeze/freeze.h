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

        int getCost() const override;
		std::string getDescription() const override;
        int getUpgradeCost() const override;
        std::pair<char, int> getType() const override;

        void upgrade() override;
};

#endif
