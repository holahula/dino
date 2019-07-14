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
				m_button_new_game("Start a new Game"),
				m_button_round("Go"),
				m_button_buy_damage_tower("Buy Damage Tower"),
				m_button_buy_freeze_tower("Buy Freeze Tower"),
				m_button_buy_money_tower("Buy Money Tower"),
				m_button_upgrade_tower("Upgrade Tower"),
				m_button_sell_tower("Sell Tower"),
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
	m_button_sell_tower.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_sell_tower_clicked));

	// Add Widgets
	box_menu.add(m_label_user_spec);
	box_menu.add(m_button_round);
	box_menu.add(m_button_new_game);

	box_shop.add(m_button_buy_damage_tower);
	box_shop.add(m_button_buy_freeze_tower);
	box_shop.add(m_button_buy_money_tower);

	box_info.add(m_label_tower_spec);
	box_info.add(m_button_upgrade_tower);
	box_info.add(m_button_sell_tower);
	
	panel_shop.add(box_shop);
	panel_info.add(box_info);
	m_grid.attach(panel_shop, 2, 1, 1, 3);
	m_grid.attach(panel_info, 3, 0, 1, 4);
	m_grid.attach(tiles, 0, 0, 2, 4);

	panel_menu.add(box_menu);
	m_grid.attach(panel_menu, 2, 0, 1, 1);
	add(m_grid);

	startNewGame();

	show_all();
	m_button_upgrade_tower.hide();
	m_button_sell_tower.hide();
	m_button_new_game.hide();
}

View::~View() {
	for (size_t i=0; i<tileViewGrid.size(); ++i) {
        for (size_t j=0; j<tileViewGrid[i].size(); ++j) {
			delete tileViewGrid[i][j];
        }
    }
}

void View::startNewGame() {
	// Start a new game
	game = unique_ptr<State>(new State);
	vector<vector<TileView *> > temp = vector<vector<TileView*> >(game->getMap()->getWidth(), vector<TileView*>(game->getMap()->getHeight(), nullptr));
	tileViewGrid = temp;
	
	// Initialize widgets
	update_view();
	
	// Build grid of tiles using the map
	Map* map = game->getMap();
	int row = 0, col = 0, nc = map->getHeight();
	for(auto it = map->begin(); it != map->end(); ++it) {
		TileView *tileView = new TileView(&(*it));
		tileViewGrid.at(row).at(col) = tileView;
		if (tileView->type == '.') {
			tileView->drag_dest_set(listTargets);
			tileView->set_events(Gdk::BUTTON_PRESS_MASK);
			tileView->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this, &View::on_tile_clicked), tileView));
		} else {
			tileView->label.set_markup("<span color=\"red\">1\n</span><span color=\"blue\">2\n</span><span color=\"yellow\">5</span>");
		}
		tileView->signal_drag_data_received().connect(sigc::bind(sigc::mem_fun(*this, &View::on_label_drop_drag_data_received), tileView));
		tileView->get_style_context()->add_class(tileView->type == '.' ? "land_tile" : "path_tile");
		tiles.attach(*tileViewGrid.at(row).at(col), row, col, 1, 1);			
		
		if (col < nc - 1) {
			col++;
		} else {
			row++;
			col = 0;
		}
	}
}

void View::on_button_new_game_clicked() {
	startNewGame();
}

void View::on_button_round_clicked() {
	m_button_round.set_sensitive(false);
	//game->startRound();
	startRound();
	m_button_round.set_sensitive();
}

void View::on_button_upgrade_tower_clicked() {
	int row = selected_tileView->row;
	int col = selected_tileView->col;
	char t = selected_tileView->type;
	if (!game->upgradeTower(col, row)) {
		string tower = getTowerFullType(t, true).first;
		Gtk::MessageDialog dialog(*this, "Invalid Upgrade", false /* use_markup */, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
		dialog.set_secondary_text(tower + " tower @ (" + to_string(col) + ", " + to_string(row) + ") could not be upgraded!");
		dialog.run();
	} else {
		update_info();
		update_tooltip();
	}
	update_view();
}

void View::on_button_sell_tower_clicked() {
	int row = selected_tileView->row;
	int col = selected_tileView->col;
	char t = selected_tileView->type;
	string tower = getTowerFullType(selected_tower->getType().first, false).first;
	if (!game->sellTower(col, row)) {
		string tower = getTowerFullType(t, true).first;
		Gtk::MessageDialog dialog(*this, "Invalid Sale", false /* use_markup */, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
		dialog.set_secondary_text(tower + " tower @ (" + to_string(col) + ", " + to_string(row) + ") could not be sold!");
		dialog.run();
	} else {
		selected_tileView->get_style_context()->remove_class(tower + "_tower_tile");
		update_selected_tower();
		update_info();
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
			tower = getTowerFullType(t, true).first;
			Gtk::MessageDialog dialog(*this, "Invalid Purchase", false /* use_markup */, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
			dialog.set_secondary_text(tower + " tower @ (" + to_string(col) + ", " + to_string(row) + ") could not be bought!");
			dialog.run();
		} else {
			tower = getTowerFullType(t, false).first;
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
	pair<string, string> type = getTowerFullType(selected_tower->getType().first, true);
	selected_tileView->label.set_tooltip_text(type.first + " tower\n" + type.second + ": " + to_string(selected_tower->getType().second));
}

void View::update_info() {
	if(selected_tower == nullptr) {
		m_label_tower_spec.set_text("Select a Tower");
		m_button_upgrade_tower.hide();
		m_button_sell_tower.hide();
	} else {
		pair<char, int> t = selected_tower->getType();
		pair<string, string> type = getTowerFullType(t.first, true);
		m_label_tower_spec.set_text(type.first + " Tower\nDescription: it's great lol\nCost: $" + to_string(selected_tower->getCost()) + "\nRange: " + to_string(selected_tower->getRange()) + "\nUpgrade Cost: " + to_string(selected_tower->getUpgradeCost()) + "\n" + type.second + ": " + to_string(t.second));
		m_button_upgrade_tower.show();
		m_button_sell_tower.show();
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

pair<string, string> View::getTowerFullType(char type, bool isCapitalized) {
	if(isCapitalized) {
		return (type == 'D') ? make_pair("Damage", "Damage") : ((type == 'F') ? make_pair("Freeze", "Duration") : make_pair("Money", "Income"));
	}
	return (type == 'D') ? make_pair("damage", "damage") : ((type == 'F') ? make_pair("freeze", "duration") : make_pair("money", "income"));
}

void View::startRound() {
    int frame = 1;
    bool status;
    int size = game->constructEnemies();
	int hp = game->hp;
    int hpStartRound = hp;
    int totalEnemyHP = game->totalHP(game->enemies);
    // round while loop 
    while(game->enemies.size() != 0){
        status = game->preFrame(frame, size);
        if(!status) break;

        game->processFrame();
        
		// display

		
    	game->map->detachAllEnemies();

        frame++;
    }

    updateState(hp, hpStartRound - hp, (double)(hpStartRound - hp)/(double)totalEnemyHP);
}

void View::updateState(int hp, int hpLost, double remainingEnemyHP) {
    if(hp <= 0) {
        Gtk::MessageDialog dialog(*this, "Game Over", false /* use_markup */, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
		dialog.set_secondary_text("You lost!");
		dialog.run();
        return;
    } else if(game->round == game->MAX_ROUND){
        Gtk::MessageDialog dialog(*this, "Game Finished", false /* use_markup */, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
		dialog.set_secondary_text("You won! There are no more rounds left.");
		dialog.run();
        return;
    }

    game->getRoundIncome();
    // spawner interactions
    game->spawner->updateState(game->round, hpLost,remainingEnemyHP);

	update_view();
    
    game->round++;
}
