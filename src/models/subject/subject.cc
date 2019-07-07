#include "subject.h"

Subject::Subject() {}
Subject::~Subject() {}

void Subject::attach(Observer *o) {
    observers.push_back(o);
}

void Subject::detach(Observer *o) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == o) {
            observers.erase(it);
            break;
        }
    }
}

void Subject::detachAll() {
    observers.clear();
}

void Subject::notifyObservers(Tower * tower) {
    // for (auto ob : observers) ob->notify(tower);
    // only notify the 1st enemy (closest to the exit)

    // TODO: if increase the # of enemies shooting, simply increase the # of notified enemies
    if(observers.size() > 0) {
        observers[0]->notify(tower);
    }
}
