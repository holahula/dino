#ifndef INVISIBLE_H_
#define INVISIBLE_H_

#include "./../enemy.h"

class InvisibleEnemy : public Enemy {
    bool visibility;

    public:
        InvisibleEnemy(int);

        void updateVisibility();
        char getType() const override;
        
        ~InvisibleEnemy();
};

#endif