#ifndef INVISIBLE_H_
#define INVISIBLE_H_

#include "./../enemy.h"

class Tower;
class InvisibleEnemy : public Enemy {
    bool visible;

    public:
        InvisibleEnemy(int);

        void updateVisibility();
        
        char getType() const override;

        virtual void nextState() override;

        virtual void notify(Tower*) override;

        ~InvisibleEnemy();
};

#endif