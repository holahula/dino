#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include <vector>
#include "./../observer/observer.h"

class Subject {
    std::vector<Observer *> observers;

public:
    Subject();
    void attach(Observer *o);
    void detach(Observer *o);
    void detachAll();
    int notifyObservers(Tower *t);
    virtual ~Subject() = 0;
};

#endif
