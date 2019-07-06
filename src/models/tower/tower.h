#ifndef TOWER_H_
#define TOWER_H_

#include "./../subject/subject.h"

class Tower : public Subject {
    private:
        int cost;

    public:
        Tower();

        virtual char getType() = 0;
        virtual void attack() = 0;

        ~Tower();
};

#endif
