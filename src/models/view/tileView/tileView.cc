#include "tileView.h"

#include <iostream>

using namespace std;

TileView::TileView(Tile *t) : Gtk::EventBox(), label("\t\t\n\t\t\n\t\t\n") {
	tile = t;
	type = t->getType();
	row = t->location().second;
	col = t->location().first;

	add(label);
}

TileView::~TileView() {}
