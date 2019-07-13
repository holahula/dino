#ifndef TILEVIEW_H_
#define TILEVIEW_H_

#include "../../tile/tile.h"
#include "../view.h"

#include <gtkmm.h>
#include <memory>

class TileView : public Gtk::EventBox {
  public:
	Tile *tile;
	Gtk::Label label;
	char type;
	int row, col;
	TileView(Tile *);
	~TileView();
};

#endif