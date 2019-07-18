#ifndef DAMAGE_H_
#define DAMAGE_H_

#include "./../tower.h"

#include <string>

class DamageTower : public Tower {
    private:
        int dmg, upCost;
		int getUpgradeCost_() const override;
        int getCost_() const override;
        std::string getDescription_() const override;
        
        std::pair<char, int> getType_() const override;

        void upgrade_() override;

    public:
        static const int cost = 10;

        DamageTower();
        virtual ~DamageTower();
};

#endif
