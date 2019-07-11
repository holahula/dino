#include "tileView.h"

using namespace std;

TileView::TileView(Tile *t) : Gtk::Label("\t\t\n\t\t\n") {
	tile = t;
	type = t->getType();
	row = t->location().second;
	col = t->location().first;
}

TileView::~TileView() {}
