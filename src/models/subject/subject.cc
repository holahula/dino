#include "subject.h"

#include <iostream>
#include <utility>

using namespace std;

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

int Subject::notifyObservers(Tower * tower) {
    int dmg = 0;
    if(observers.size() > 0) {
        try {
            Observer* val = observers.at(0);
            dmg = val->notify(tower);
        }  catch (out_of_range& err) {
            cout << "out of range! cannot shoot this enemy" << endl;
        }
    }

    return dmg;
}
