#ifndef REGENERATIVE_H_
#define REGENERATIVE_H_

#include "./../enemy.h"

class RegenerativeEnemy : public Enemy {
    private:
        int maxHP, regen;

        char getType_() const override;
        void nextState_() override;

    public:
        RegenerativeEnemy(int, int);
        virtual ~RegenerativeEnemy();

        void heal();
};

#endif