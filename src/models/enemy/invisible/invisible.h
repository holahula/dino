#ifndef INVISIBLE_H_
#define INVISIBLE_H_

#include "./../enemy.h"

class Tower;
class InvisibleEnemy : public Enemy {

        char getType_() const override;
        void nextState_() override;
    
    public:
        InvisibleEnemy(int);

        void updateVisibility();
        
 

        ~InvisibleEnemy();
};

#endif