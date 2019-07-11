#ifndef INVISIBLE_H_
#define INVISIBLE_H_

#include "./../enemy.h"

class Tower;
class InvisibleEnemy : public Enemy {
    public:
        InvisibleEnemy(int);

        void updateVisibility();
        
        char getType() const override;

        virtual void nextState() override;

        ~InvisibleEnemy();
};

#endif