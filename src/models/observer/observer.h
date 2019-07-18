#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Tower;

class Observer {
    virtual int notify_(Tower*) = 0;
    public:
        int notify(Tower*);
        virtual ~Observer();
};

#endif
