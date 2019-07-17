#ifndef BASIC_H_
#define BASIC_H_

#include "./../enemy.h"

class BasicEnemy : public Enemy {
        char getType_() const override;
        void nextState_() override;

    public:
        BasicEnemy(int);
        virtual ~BasicEnemy();
};

#endif 