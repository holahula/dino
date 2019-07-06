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
        // private b/c movement is controlled by game state
        int isFrozen;

    protected:
        int hp;

    public:
        Enemy(int);

        void decFrozen();
        int getFrozen() const;

        virtual char getType() const = 0;
        
        void notify(Tower *tower) override;
        void observeTowers(std::vector<Tower*>);
        void removeTowers();

        virtual ~Enemy();
};

#endif
