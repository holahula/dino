#include "map.h"

using namespace std;

Map::Iterator::Iterator(int col, int row, int nc, int nr, Map* m) : col(row), row(col), nc(nc), nr(nr), m(m) {}

Tile* Map::Iterator::operator*() {
    cout << col << " " << row << endl;
	return m->p->map.at(col).at(row);
}

Map::Iterator& Map::Iterator::operator++() {
	if (col < nc - 1) {
      ++col;
    } else {
        col = 0;
        --row;
    }
	return *this;
}

bool Map::Iterator::operator!=(const Map::Iterator &other) {
	return row != other.row || col != other.col;
}