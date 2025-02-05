#ifndef TILEVIEW_H_
#define TILEVIEW_H_

#include "../../map/tile/tile.h"
#include "../view.h"

#include <gtkmm.h>
#include <memory>

class TileView : public Gtk::EventBox {
  public:
    Gtk::Label label;
    int row, col;
    char type;
    TileView(int, int, char);
    ~TileView();
};

#endif