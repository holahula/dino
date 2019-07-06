#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Tower;

class Observer {
    public:
        virtual void notify(Tower*) = 0;
        virtual ~Observer();
};

#endif
