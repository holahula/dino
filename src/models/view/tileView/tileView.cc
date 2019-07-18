#include "tileView.h"

#include <iostream>

using namespace std;

TileView::TileView(int row, int col, char type) : Gtk::EventBox(), label("\t\t\n\t\t\n\t\t\n"), row(row), col(col), type(type) {
    add(label);
}

TileView::~TileView() {}
