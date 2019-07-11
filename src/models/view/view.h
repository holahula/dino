#ifndef VIEW_H
#define VIEW_H

#include "../state/state.h"
#include "tileView/tileView.h"

#include <gtkmm.h>

class View : public Gtk::Window {
  	State game;
	// targets for drag and drop
	std::vector<Gtk::TargetEntry> listTargets;

 protected:
	Gtk::Grid m_grid;
	Gtk::Grid tiles;
	Gtk::Frame panel_menu;
	Gtk::Frame panel_shop;
	Gtk::ButtonBox bbox_menu;
	Gtk::ButtonBox bbox_shop;
	Gtk::Button m_button_new_game;
	Gtk::Button m_button_round;
	Gtk::Button m_button_buy_damage_tower;
	Gtk::Button m_button_buy_freeze_tower;
	Gtk::Button m_button_buy_money_tower;
	Gtk::Label m_label_money;
	
	void on_button_new_game_clicked();
	void on_button_round_clicked();
	void on_button_buy_damage_tower_clicked();
	void on_button_buy_freeze_tower_clicked();
	void on_button_buy_money_tower_clicked();

	void on_button_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time);
	void on_label_drop_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time, TileView *tileView);

 public:
	View();
	virtual ~View();
	int x = 0;
	int y = 0;
};

#endif
