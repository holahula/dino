#include "tileView.h"

using namespace std;

TileView::TileView(Tile *t) : Gtk::Label("\t\t\n\t\t\n") {
	tile = t;
}

TileView::~TileView() {}
