#include "map.h"

using namespace std;

Map::Iterator::Iterator(int row, int col, int nr, int nc, vector<vector<Tile*> > map) : row{row}, col{col}, nr{nr}, nc{nc}, map{map} {}

char Map::Iterator::operator*() {
	return map.at(row).at(col)->getType();
}

Map::Iterator& Map::Iterator::operator++() {
	if (col < nc - 1) {
      col++;
    } else {
      row++;
      col = 0;
	}
	return *this;
}

bool Map::Iterator::operator!=(const Map::Iterator &other) {
	return row != other.row || col != other.col;
}

Map::Iterator Map::begin() {
	return Iterator{0, 0, p->width, p->height, p->map};
}

Map::Iterator Map::end() {
	return Iterator{p->width, 0, p->width, p->height, p->map};
}