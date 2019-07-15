#ifndef TOWER_H_
#define TOWER_H_

#include "./../subject/subject.h"

#include <string>

class Tower : public Subject {
    protected:
        int range;
        int totalValue;
    public:
        Tower(int);
        ~Tower();

        virtual std::pair<char, int> getType() const = 0;
        virtual int getCost() const = 0;
        virtual std::string getDescription() const = 0;
        virtual int getUpgradeCost() const = 0;
        int getTotalValue() const;
        virtual void upgrade() = 0;

        int getRange() const;
        void attack();
};

#endif
