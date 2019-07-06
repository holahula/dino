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

    protected:
        int hp;
        bool isSlowed;

    public:
        Enemy(int);

        void notify(Tower * tower) override;

        virtual char getType() const = 0;

        void observeTowers(std::vector<Tower*>);
        void removeTowers();

        virtual ~Enemy();
};

#endif
