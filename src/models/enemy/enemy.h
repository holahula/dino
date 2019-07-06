#ifndef ENEMY_H_
#define ENEMY_H_

<<<<<<< HEAD
#include <vector>
#include <iostream>

#include "./../observer/observer.h"
#include "./../tower/tower.h"

class Enemy : public Observer {        
    private:
        Tower *subject;

    protected:
        int hp;
        int x, y;

    public:
        Enemy(int, int, int);

        void notify() override;

        void updatePos(int, int);
        virtual char GetType() const = 0;

        virtual ~Enemy();
};

class BasicEnemy : public Enemy {

    public:
        BasicEnemy(int, int, int);

};

class RegenerativeEnemy : public Enemy {
    private:
        int regen;

    public:
        RegenerativeEnemy(int, int, int);

        void heal();

};

class InvisibleEnemy : public Enemy {
    private:
        bool visibility;
        
    public:
        InvisibleEnemy(int, int, int);
        void updateVisibility();
=======

class Enemy {
    // TODO
>>>>>>> 1f6623240002e6dfad9835f7cada00dc7fbf157d
};

#endif
