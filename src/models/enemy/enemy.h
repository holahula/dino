#ifndef ENEMY_H_
#define ENEMY_H_

#include "./../observer/observer.h"
#include "./../tower/tower.h"
#include "./../subject/subject.h"

#include <vector>
#include <iostream>

class Enemy : public Observer {        
    private:
        std::vector<Tower*> subjects;
        int isFrozen;

        virtual char getType_() const = 0;
        virtual void nextState_() = 0;
        virtual int notify_(Tower*) override; 

    protected:
        int hp;
        bool targetable;

    public:
        Enemy(int);
        virtual ~Enemy();

        int getHP() const;

        void decFrozen();
        int getFrozen() const;

        char getType();
        void nextState();

        void observeTowers(std::vector<Tower*>);

        void removeTowers();
        bool isTargetable() const;

        friend std::ostream& operator<<(std::ostream&, Enemy&);
};

#endif
