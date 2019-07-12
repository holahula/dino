#include "map.h"

using namespace std;

Map::Iterator::Iterator(int row, int col, int nr, int nc, vector<vector<Tile*> > map) : row(row), col{col}, nr{nr}, nc{nc}, map{map} {
	cout << "constructor" << endl;
}

Tile& Map::Iterator::operator*() {
	cout << "operator*" << endl;
	return *(map.at(row).at(col));
}

Map::Iterator& Map::Iterator::operator++() {
	cout << "operator++" << endl;
	if (col < nc - 1) {
      col++;
    } else {
      row++;
      col = 0;
	}
	return *this;
}

bool Map::Iterator::operator!=(const Map::Iterator &other) {
	cout << "operator!=" << endl;
	return row != other.row || col != other.col;
}

Map::Iterator Map::begin() {
	cout << "begin" << endl;
	return Iterator{0, 0, width, height, map};
}

Map::Iterator Map::end() {
	cout << "end" << endl;
	return Iterator{width, 0, width, height, map};
}