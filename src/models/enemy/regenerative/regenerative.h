#ifndef REGENERATIVE_H_
#define REGENERATIVE_H_

#include "./../enemy.h"

class RegenerativeEnemy : public Enemy {
    private:
        int maxHP, regen;

    public:
        RegenerativeEnemy(int, int);
        void heal();

        char getType() const override;
        
        void nextState() override;

        ~RegenerativeEnemy();
};

#endif