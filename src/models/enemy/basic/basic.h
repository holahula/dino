#ifndef BASIC_H_
#define BASIC_H_

#include "./../enemy.h"

class BasicEnemy : public Enemy {
    public:
        BasicEnemy(int);

        char getType() const override;

        void nextState() override;

        ~BasicEnemy();
};

#endif 