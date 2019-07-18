#ifndef TOWER_H_
#define TOWER_H_

#include "./../subject/subject.h"

#include <string>

class Tower : public Subject {
	private:
		virtual std::pair<char, int> getType_() const = 0;
        virtual int getCost_() const = 0;
        virtual std::string getDescription_() const = 0;
        virtual int getUpgradeCost_() const = 0;
        virtual void upgrade_() = 0;

    protected:
        int range;
        int totalValue;

    public:
        Tower(int);
        virtual ~Tower();

        std::pair<char, int> getType() const;
        int getCost() const;
        std::string getDescription() const;
        int getUpgradeCost() const;
        void upgrade();

        int getTotalValue() const;

        int getRange() const;
        void attack();
};

#endif
