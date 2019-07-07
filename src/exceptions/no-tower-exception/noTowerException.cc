#include "noTowerException.h"
#include <string>
using namespace std;

NoTowerException::NoTowerException(string msg): msg(msg){}

string NoTowerException::what() {
    return msg;
}
