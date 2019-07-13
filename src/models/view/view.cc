#include "view.h"
#include "tileView/tileView.h"
#include "../enemy/enemy.h"
#include "../map/map.h"
#include "../map/tile/tile.h"
#include "../map/tile/land/landTile.h"
#include "../map/tile/path/pathTile.h"
#include "../state/state.h"
#include "../tower/tower.h"
#include "../../exceptions/no-tower-exception/noTowerException.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

using namespace std;

View::View() : selected_tileView(nullptr),
				selected_tower(nullptr),
				panel_menu("Menu"),
				panel_shop("Shop"),
				panel_info("Information"),
				box_menu(Gtk::ORIENTATION_VERTICAL),
				box_shop(Gtk::ORIENTATION_VERTICAL),
				box_info(Gtk::ORIENTATION_VERTICAL),
				m_button_new_game("New Game"),
				m_button_round("Start Next Round"),
				m_button_buy_damage_tower("Buy Damage Tower"),
				m_button_buy_freeze_tower("Buy Freeze Tower"),
				m_button_buy_money_tower("Buy Money Tower"),
				m_button_upgrade_tower("Upgrade Tower"),
				m_label_tower_spec("Select a Tower") {

	// Customizations
	set_title("Tower Defense");
	set_border_width(10);

	box_menu.set_border_width(5);
	box_menu.set_layout(Gtk::BUTTONBOX_SPREAD);
	box_menu.set_spacing(5);

	box_shop.set_border_width(5);
	box_shop.set_layout(Gtk::BUTTONBOX_SPREAD);
	box_shop.set_spacing(5);

	box_info.set_border_width(5);
	box_info.set_layout(Gtk::BUTTONBOX_SPREAD);
	box_info.set_spacing(5);

	tiles.set_row_spacing(0);
	tiles.set_column_spacing(0);

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

	// Customize drag and drop icon
	Glib::RefPtr<Gdk::Pixbuf> icon_damage_tower = Gdk::Pixbuf::create_from_file("resources/assets/damage_tower.png");
	m_button_buy_damage_tower.drag_source_set_icon(icon_damage_tower);
	Glib::RefPtr<Gdk::Pixbuf> icon_freeze_tower = Gdk::Pixbuf::create_from_file("resources/assets/freeze_tower.png");
	m_button_buy_freeze_tower.drag_source_set_icon(icon_freeze_tower);
	Glib::RefPtr<Gdk::Pixbuf> icon_money_tower = Gdk::Pixbuf::create_from_file("resources/assets/money_tower.png");
	m_button_buy_money_tower.drag_source_set_icon(icon_money_tower);

	// Signals:
	m_button_buy_damage_tower.signal_drag_data_get().connect(sigc::bind(sigc::mem_fun(*this, &View::on_button_drag_data_get), 'D'));
	m_button_buy_freeze_tower.signal_drag_data_get().connect(sigc::bind(sigc::mem_fun(*this, &View::on_button_drag_data_get), 'F'));
	m_button_buy_money_tower.signal_drag_data_get().connect(sigc::bind(sigc::mem_fun(*this, &View::on_button_drag_data_get), 'M'));
	
	m_button_new_game.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_new_game_clicked));
	m_button_round.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_round_clicked));
	m_button_upgrade_tower.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_upgrade_tower_clicked));

	// Add buttons and labels to boxes
	box_menu.add(m_button_new_game);
	box_menu.add(m_label_user_spec);
	box_menu.add(m_button_round);

	box_shop.add(m_button_buy_damage_tower);
	box_shop.add(m_button_buy_freeze_tower);
	box_shop.add(m_button_buy_money_tower);

	box_info.add(m_label_tower_spec);
	box_info.add(m_button_upgrade_tower);

	// Add widgets to grids
	
	panel_shop.add(box_shop);
	panel_info.add(box_info);
	m_grid.attach(panel_shop, 2, 1, 1, 1);
	m_grid.attach(panel_info, 2, 2, 1, 2);
	m_grid.attach(tiles, 0, 0, 2, 4);

	panel_menu.add(box_menu);
	m_grid.attach(panel_menu, 2, 0, 1, 1);
	add(m_grid);

	show_all();
	m_button_upgrade_tower.hide();
}

View::~View() {}

void View::on_button_new_game_clicked() {
	// Start a new game
	game = unique_ptr<State>(new State);

	// Initialize widgets
	// m_price_damage_tower.set_text("Price: " + to_string(game->shop->))
	update_view();
	
	// Build grid of tiles using the map
	Map* map = game->getMap();
	int row = 0, col = 0, nc = map->getHeight();
	for(auto it = map->begin(); it != map->end(); ++it) {
		TileView* tileView = new TileView(&(*it));
		if (tileView->type == '.') {
			tileView->drag_dest_set(listTargets);
			tileView->set_events(Gdk::BUTTON_PRESS_MASK);
			tileView->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this, &View::on_tile_clicked), tileView));
		} else {
			tileView->label.set_markup("<span color=\"red\">1\n</span><span color=\"blue\">2\n</span><span color=\"yellow\">5</span>");
		}
		tileView->signal_drag_data_received().connect(sigc::bind(sigc::mem_fun(*this, &View::on_label_drop_drag_data_received), tileView));
		tileView->get_style_context()->add_class(tileView->type == '.' ? "land_tile" : "path_tile");
		tiles.attach(*tileView, row, col, 1, 1);			
		
		if (col < nc - 1) {
			col++;
		} else {
			row++;
			col = 0;
		}
	}

	show_all();
	m_button_upgrade_tower.hide();
}

void View::on_button_round_clicked() {
	game->startRound();
	update_view();
}

void View::on_button_upgrade_tower_clicked() {
	int row = selected_tileView->row;
	int col = selected_tileView->col;
	char t = selected_tileView->type;
	if (!game->upgradeTower(col, row)) {
		string tower = (t == 'D') ? "Damage" : ((t == 'F') ? "Freeze" : "Money");
		Gtk::MessageDialog dialog(*this, "Invalid Upgrade", false /* use_markup */, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
		dialog.set_secondary_text(tower + " tower @ (" + to_string(col) + ", " + to_string(row) + ") could not be upgraded!");
		dialog.run();
	} else {
		update_info();
		update_tooltip();
	}
	update_view();
}

bool View::on_tile_clicked(GdkEventButton *, TileView *tileView) {
	update_selected_tileView(tileView);
	update_info();
	return true;
}

void View::on_button_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time, char tower) {
  	selection_data.set(selection_data.get_target(), 8, (const guchar*)(&tower), 1);
}

void View::on_label_drop_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int, int, const Gtk::SelectionData& selection_data, guint info, guint time, TileView *tileView) {
	const int length = selection_data.get_length();
	if((length >= 0) && (selection_data.get_format() == 8))
	{
		char t = (char)(*selection_data.get_data());
		int row = tileView->row;
		int col = tileView->col;
		string tower;
		if (!game->buyTower(t, col, row)) {
			tower = (t == 'D') ? "Damage" : ((t == 'F') ? "Freeze" : "Money");
			Gtk::MessageDialog dialog(*this, "Invalid Purchase", false /* use_markup */, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
			dialog.set_secondary_text(tower + " tower @ (" + to_string(col) + ", " + to_string(row) + ") could not be bought!");
			dialog.run();
		} else {
			tower = (t == 'D') ? "damage" : ((t == 'F') ? "freeze" : "money");
			tileView->get_style_context()->add_class(tower + "_tower_tile");
			update_selected_tileView(tileView);
			update_tooltip();
		}
	}
	context->drag_finish(false, false, time);
}

void View::update_view() {
	m_label_user_spec.set_text("Round: " + to_string(game->getRound()) + "\nHp: " + to_string(game->getHp()) + "\nMoney: " + to_string(game->getMoney()));
}

void View::update_tooltip() {
	pair<string, int> type = (selected_tower->getType().first == 'D') ? make_pair("Damage", selected_tower->getType().second) : ((selected_tower->getType().first == 'F') ? make_pair("Duration", selected_tower->getType().second) : make_pair("Income", selected_tower->getType().second));
	selected_tileView->label.set_tooltip_text(type.first + ": " + to_string(type.second));
}

void View::update_info() {
	if(selected_tower == nullptr) {
		m_label_tower_spec.set_text("Select a Tower");
		m_button_upgrade_tower.hide();
	} else {
		pair<char, int> t = selected_tower->getType();
		pair<string, string> type = (t.first == 'D') ? make_pair("Damage", "Damage") : ((t.first == 'F') ? make_pair("Freeze", "Duration") : make_pair("Money", "Income"));
		m_label_tower_spec.set_text(type.first + " Tower\nDescription: it's great lol\nCost: $" + to_string(selected_tower->getCost()) + "\nRange: " + to_string(selected_tower->getRange()) + "\nUpgrade Cost: " + to_string(selected_tower->getUpgradeCost()) + "\n" + type.second + ": " + to_string(t.second));
		m_button_upgrade_tower.show();
	}
}

void View::update_selected_tileView(TileView *tileView) {
	selected_tileView = tileView;
	update_selected_tower();
}

void View::update_selected_tower() {
	try {
		selected_tower = game->getMap()->getTower(selected_tileView->col, selected_tileView->row);
	} catch(NoTowerException e) {
		selected_tower = nullptr;
		return;
	}
}
