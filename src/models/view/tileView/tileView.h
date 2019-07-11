#ifndef TILEVIEW_H_
#define TILEVIEW_H_

#include "../../tile/tile.h"

#include <gtkmm.h>
#include <memory>

class TileView : public Gtk::Label {
  public:
	Tile *tile;
	char type;
	int row, col;
	TileView(Tile *);
	~TileView();
};

#endif