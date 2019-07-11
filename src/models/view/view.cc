#include "view.h"
#include "../tile/path/pathTile.h"
#include "./../enemy/enemy.h"
#include "./../map/map.h"
#include "./../state/state.h"
#include "./../tower/tower.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

View::View() : panel_menu("Menu"),
				panel_shop("Shop"),
				bbox_menu(Gtk::ORIENTATION_VERTICAL),
				bbox_shop(Gtk::ORIENTATION_VERTICAL),
				m_button_new_game("New Game"),
				m_button_print("Print Map"),
				m_button_round("Start Next Round"),
				m_button_buy_damage_tower("Buy Damage Tower"),
				m_button_buy_freeze_tower("Buy Freeze Tower"),
				m_button_buy_money_tower("Buy Money Tower") {

	// Customizations
	set_title("Tower Defense");
	set_border_width(10);

	bbox_menu.set_border_width(5);
	bbox_menu.set_layout(Gtk::BUTTONBOX_SPREAD);
	bbox_menu.set_spacing(10);

	bbox_shop.set_border_width(5);
	bbox_shop.set_layout(Gtk::BUTTONBOX_SPREAD);
	bbox_shop.set_spacing(10);

	m_grid.set_column_spacing(10);

	// Set up drag and drop functionality
	//Targets:
	listTargets.push_back(Gtk::TargetEntry("STRING"));
	listTargets.push_back(Gtk::TargetEntry("text/plain"));

	//Drag site:
	//Make button a DnD drag source:
	m_button_buy_damage_tower.drag_source_set(listTargets);
	m_button_buy_freeze_tower.drag_source_set(listTargets);
	m_button_buy_money_tower.drag_source_set(listTargets);

	//Connect signals:
	m_button_buy_damage_tower.signal_drag_data_get().connect(sigc::mem_fun(*this, &View::on_button_drag_data_get));
	m_button_buy_freeze_tower.signal_drag_data_get().connect(sigc::mem_fun(*this, &View::on_button_drag_data_get));
	m_button_buy_money_tower.signal_drag_data_get().connect(sigc::mem_fun(*this, &View::on_button_drag_data_get));

	// Signals
	m_button_new_game.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_new_game_clicked));
	m_button_print.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_print_clicked));
	m_button_round.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_round_clicked));
	m_button_buy_damage_tower.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_buy_damage_tower_clicked));
	m_button_buy_freeze_tower.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_buy_freeze_tower_clicked));
	m_button_buy_money_tower.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_buy_money_tower_clicked));

	// Add widgets to grids
	bbox_menu.add(m_button_new_game);
	panel_menu.add(bbox_menu);
	m_grid.attach(panel_menu, 2, 0, 1, 1);
	add(m_grid);

	show_all();
	
}

View::~View() {}

void View::on_button_new_game_clicked() {
	// Start a new game
	game = State();

	// Initialize widgets
	m_label_money.set_text("Money: " + to_string(game.getMoney()));

	//Add widgets to grid
	bbox_menu.add(m_button_print);
	bbox_menu.add(m_button_round);

	bbox_shop.add(m_button_buy_damage_tower);
	bbox_shop.add(m_button_buy_freeze_tower);
	bbox_shop.add(m_button_buy_money_tower);
	
	// Build grid of tiles using the map
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			Gtk::Label* label = Gtk::manage(new Gtk::Label("\t\t\n\t\t\n"));
			label->drag_dest_set(listTargets);
			label->signal_drag_data_received().connect(sigc::mem_fun(*this, &View::on_label_drop_drag_data_received));
			label->override_background_color(Gdk::RGBA(i * j % 2 == 0 ? "green" : "brown"), Gtk::STATE_FLAG_NORMAL);
			tiles.attach(*label, i, j, 1, 1);
		}
	}
	
	tiles.set_row_spacing(0);
	tiles.set_column_spacing(0);
	panel_shop.add(bbox_shop);
	m_grid.attach(panel_shop, 2, 1, 1, 1);
	m_grid.attach(tiles, 0, 0, 2, 2);

	show_all();
}

void View::on_button_print_clicked() {
	game.displayMap();
}

void View::on_button_round_clicked() {
	game.startRound();
}

void View::on_button_buy_damage_tower_clicked() {
	if (!game.buyTower('D', x, y)) {
		cout << "Invalid Purchase! Damage tower @ (" << x << ", " << y << ") could not be bought" << endl;
	} else {
		cout << "Damage tower purchase successful!" << endl;
	}
}

void View::on_button_buy_freeze_tower_clicked() {
	if (!game.buyTower('F', x, y)) {
		cout << "Invalid Purchase! Freeze tower @ (" << x << ", " << y << ") could not be bought" << endl;
	} else {
		cout << "Freeze tower purchase successful!" << endl;
	}
}

void View::on_button_buy_money_tower_clicked() {
	if (!game.buyTower('M', x, y)) {
		cout << "Invalid Purchase! Money tower @ (" << x << ", " << y << ") could not be bought" << endl;
	} else {
		cout << "Money tower purchase successful!" << endl;
	}
}

void View::on_button_drag_data_get(
    const Glib::RefPtr<Gdk::DragContext>&,
    Gtk::SelectionData& selection_data, guint, guint) {
  	selection_data.set(selection_data.get_target(), 8 /* 8 bits format */,
                     (const guchar*)"I'm Data!",
                     9 /* the length of I'm Data! in bytes */);
}

void View::on_label_drop_drag_data_received(
    const Glib::RefPtr<Gdk::DragContext>& context, int, int,
    const Gtk::SelectionData& selection_data, guint, guint time) {
	const int length = selection_data.get_length();
	if ((length >= 0) && (selection_data.get_format() == 8)) {
		std::cout << "Received \"" << selection_data.get_data_as_string() << "\" in label " << std::endl;
	}

	context->drag_finish(false, false, time);
}
