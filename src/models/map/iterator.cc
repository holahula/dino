#include "map.h"

using namespace std;

Map::Iterator::Iterator(int col, int row, int nc, int nr, Map* m) : col(row), row(col), nc(nc), nr(nr), m(m) {}

Tile* Map::Iterator::operator*() {
	return m->p->map.at(col).at(row);
}

Map::Iterator& Map::Iterator::operator++() {
	if (col < nc - 1) {
      ++col;
    } else {
      --row;
      col = 0;
	}
	return *this;
}

bool Map::Iterator::operator!=(const Map::Iterator &other) {
	return row != other.row || col != other.col;
}

Map::Iterator Map::begin() {
	return Iterator(0, p->height-1, p->width, p->height, this);
}

Map::Iterator Map::end() {
	return Iterator(0, -1, p->width, p->height, this);
} 