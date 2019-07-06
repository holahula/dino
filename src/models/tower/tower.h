#ifndef TOWER_H_
#define TOWER_H_

#include "./../subject/subject.h"

class Tower : public Subject {
    public:
        Tower();
        ~Tower();

        virtual std::pair<char, int> getType() const = 0;
        virtual void upgrade() = 0;

        void attack();
};

#endif
