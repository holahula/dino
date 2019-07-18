#include "observer.h"

int Observer::notify(Tower* tower) {
	return notify_(tower);
}

Observer::~Observer() {}
