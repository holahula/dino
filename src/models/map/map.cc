#include "map.h"

using namespace std;

Map::Map(){}
Map::~Map(){}

ostream& operator<<(ostream& out, Map & currFrame) {
    for (int i=0; i<currFrame.map.size(); ++i) {
        for (int j=0; j<currFrame.map[i].size(); ++j) {
            out << currFrame.map[i][j]->getType() << " ";
        }
        out << endl;
    }
    return out;
}