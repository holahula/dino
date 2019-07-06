#include "landTileOccupiedException.h"
#include <string>
using namespace std;

LandTileOccupiedException::LandTileOccupiedException(string msg): msg(msg){}

string LandTileOccupiedException::what() {
    return msg;
}
