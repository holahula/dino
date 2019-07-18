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

View::View(bool adaptive, bool map) : selected_tileView(nullptr),
                selected_tower(nullptr),
                box_menu(Gtk::ORIENTATION_HORIZONTAL),
                box_round(Gtk::ORIENTATION_VERTICAL),
                box_shop(Gtk::ORIENTATION_VERTICAL),
                box_info(Gtk::ORIENTATION_VERTICAL),
                frame_menu("Menu"),
                frame_shop("Shop"),
                frame_info("Information"),
                m_button_round("Go"),
                m_button_next("Next"),
                m_button_skip("Skip"),
                m_button_buy_damage_tower("Buy Damage Tower"),
                m_button_buy_freeze_tower("Buy Freeze Tower"),
                m_button_buy_money_tower("Buy Money Tower"),
                m_button_upgrade_tower("Upgrade Tower"),
                m_button_sell_tower("Sell Tower"),
                m_label_damage_tower(""),
                m_label_freeze_tower(""),
                m_label_money_tower(""),
                m_label_tower_spec("Select a Tower") {

    // Customizations
    set_title("Dino Tower Defense");
    set_border_width(10);
    get_style_context()->add_class("window");

    box_menu.set_border_width(10);
    box_menu.set_layout(Gtk::BUTTONBOX_SPREAD);
    box_menu.set_spacing(10);

    box_round.set_layout(Gtk::BUTTONBOX_SPREAD);
    box_round.set_spacing(10);

    box_shop.set_border_width(10);
    box_shop.set_layout(Gtk::BUTTONBOX_SPREAD);
    box_shop.set_spacing(10);

    box_info.set_border_width(10);
    box_info.set_layout(Gtk::BUTTONBOX_SPREAD);
    box_info.set_spacing(10);

    frame_menu.set_border_width(10);
    frame_shop.set_border_width(10);
    frame_info.set_border_width(10);

    tiles.set_border_width(10);
    tiles.set_row_spacing(0);
    tiles.set_column_spacing(0);

    box_menu.get_style_context()->add_class("box");
    box_shop.get_style_context()->add_class("box");
    box_info.get_style_context()->add_class("box");

    frame_menu.get_style_context()->add_class("frame");
    frame_shop.get_style_context()->add_class("frame");
    frame_info.get_style_context()->add_class("frame");

    m_grid.set_column_spacing(0);
    m_grid.set_row_spacing(0);

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
    
    m_button_round.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_round_clicked));
    m_button_next.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_next_clicked));
    m_button_skip.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_skip_clicked));
    m_button_upgrade_tower.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_upgrade_tower_clicked));
    m_button_sell_tower.signal_clicked().connect(sigc::mem_fun(*this, &View::on_button_sell_tower_clicked));

    // Add Widgets
    box_menu.add(m_label_user_spec);
    box_menu.add(box_round);

    box_round.add(m_button_round);
    box_round.add(m_button_next);
    box_round.add(m_button_skip);

    box_shop.add(m_button_buy_damage_tower);
    box_shop.add(m_label_damage_tower);
    box_shop.add(m_button_buy_freeze_tower);
    box_shop.add(m_label_freeze_tower);
    box_shop.add(m_button_buy_money_tower);
    box_shop.add(m_label_money_tower);

    box_info.add(m_label_tower_spec);
    box_info.add(m_button_upgrade_tower);
    box_info.add(m_button_sell_tower);

    frame_menu.add(box_menu);
    frame_shop.add(box_shop);
    frame_info.add(box_info);

    m_grid.attach(frame_info, 1, 3, 1, 9);
    m_grid.attach(tiles, 0, 0, 1, 12);
    m_grid.attach(frame_shop, 2, 0, 1, 12);
    m_grid.attach(frame_menu, 1, 0, 1, 3);

    add(m_grid);

    startNewGame(adaptive, map);

    show_all();
    m_button_upgrade_tower.hide();
    m_button_sell_tower.hide();
    m_button_next.set_sensitive(false);
    m_button_skip.set_sensitive(false);
}

View::~View() {}

void View::startNewGame(bool adaptive, bool customPath) {
    // Start a new game
    game = unique_ptr<State>(new State(adaptive, customPath));
    
    // Initialize widgets
    update_view();
    m_label_damage_tower.set_text("Cost: $" + to_string(game->getTowerFromShop('D')->getCost()) + "\nDescription:\n" + game->getTowerFromShop('D')->getDescription());
    m_label_freeze_tower.set_text("Cost: $" + to_string(game->getTowerFromShop('F')->getCost()) + "\nDescription:\n" + game->getTowerFromShop('F')->getDescription());
    m_label_money_tower.set_text("Cost: $" + to_string(game->getTowerFromShop('M')->getCost()) + "\nDescription:\n" + game->getTowerFromShop('M')->getDescription());
    
    // Build grid of tiles using the map
    Map* map = game->getMap();
    for (Tile* t: *map) {
        int row = t->location().second;
        int col = t->location().first;
        TileView *tileView = new TileView(row, col, t->getType());
        if (tileView->type == '.') {
            tileView->drag_dest_set(listTargets);
        } else {
            tileViewPath.emplace_back(tileView);
        }
        tileView->set_events(Gdk::BUTTON_PRESS_MASK);
        tileView->signal_button_press_event().connect(sigc::bind(sigc::mem_fun(*this, &View::on_tile_clicked), tileView));
        tileView->signal_drag_data_received().connect(sigc::bind(sigc::mem_fun(*this, &View::on_label_drop_drag_data_received), tileView));
        tileView->get_style_context()->add_class(tileView->type == '.' ? "land_tile" : "path_tile");
        tiles.attach(*tileView, col, map->getHeight()-row-1, 1, 1);
    }
}

void View::on_button_round_clicked() {
    m_button_round.set_sensitive(false);
    m_button_buy_damage_tower.set_sensitive(false);
    m_button_buy_freeze_tower.set_sensitive(false);
    m_button_buy_money_tower.set_sensitive(false);
    m_button_upgrade_tower.set_sensitive(false);
    m_button_sell_tower.set_sensitive(false);
    m_button_next.set_sensitive();
    m_button_skip.set_sensitive();
    startRound();
}

void View::on_button_next_clicked() {
    nextStep();
}

void View::on_button_skip_clicked() {
    while(roundDone == false) {
        nextStep();
    }
    nextStep();
}

void View::on_button_upgrade_tower_clicked() {
    int row = selected_tileView->row;
    int col = selected_tileView->col;
    char t = selected_tileView->type;
    if (!game->upgradeTower(col, row)) {
        string tower = getTowerFullType(t, true).first;
        displayPopup("Invalid Upgrade", tower + " tower @ (" + to_string(col) + ", " + to_string(row) + ") could not be upgraded!", true);
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
        displayPopup("Invalid Sale", tower + " tower @ (" + to_string(col) + ", " + to_string(row) + ") could not be sold!", true);
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
            displayPopup("Invalid Purchase", tower + " tower @ (" + to_string(col) + ", " + to_string(row) + ") could not be bought!", true);
        } else {
            tower = getTowerFullType(t, false).first;
            tileView->get_style_context()->add_class(tower + "_tower_tile");
            update_selected_tileView(tileView);
            update_view();
            update_tooltip();
        }
    }
    context->drag_finish(false, false, time);
}

void View::update_view() {
    m_label_user_spec.set_text("Round: " + to_string(game->getRound()) + "\nHp: " + to_string(game->getHp()) + "\nMoney: $" + to_string(game->getMoney()));
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
        m_label_tower_spec.set_text(type.first + " Tower\n\nDescription:\n" + game->getTowerFromShop(t.first)->getDescription()  + "\n\n" + type.second + ": " + to_string(t.second) + "\nRange: " + to_string(selected_tower->getRange()) + "\nUpgrade Cost: $" + to_string(selected_tower->getUpgradeCost()));
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
        selected_tower = game->getTower(selected_tileView->col, selected_tileView->row);
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
    frame = 1;
    size = game->constructEnemies();
    hpStartRound = game->getHp();
    totalEnemyHP = game->totalHP(game->getEnemies());
    roundDone = false;
}

void View::nextStep() {
    update_view();
    if(roundDone) {
        displayEnemies();
        updateState(hpStartRound - game->getHp(), (double)(hpStartRound - game->getHp())/(double)totalEnemyHP);
        m_button_round.set_sensitive();
        m_button_buy_damage_tower.set_sensitive();
        m_button_buy_freeze_tower.set_sensitive();
        m_button_buy_money_tower.set_sensitive();
        m_button_upgrade_tower.set_sensitive();
        m_button_sell_tower.set_sensitive();
        m_button_next.set_sensitive(false);
        m_button_skip.set_sensitive(false);
        return;
    }
    status = game->preFrame(frame, size);
    if(!status) {
        roundDone = true;
    }
    displayEnemies();
    game->processFrame();
    game->getMap()->detachAllEnemies();
    frame++;
    if(game->getEnemies().size() <= 0) {
        roundDone = true;
    }
}

void View::updateState(int hpLost, double remainingEnemyHP) {
    if(game->getHp() <= 0) {
        displayPopup("Game Over", "You lost! You lost all your HP.", false);
        close();
        return;
    } else if(game->getRound() == game->MAX_ROUND){
        displayPopup("Game Finished", "You won! There are no more rounds left.", false);
        close();
        return;
    }

    if(game->getRound() % 5 == 0){
        displayPopup("Boss Level Passed!", "You gained 5 HP for killing the boss level!", false);
        int max_ = State::MAX_HP;
        game->setHp(min(max_, game->getHp()+5));
    }

    game->getRoundIncome();
    game->getSpawner()->updateState(game->getRound(), hpLost, remainingEnemyHP);

    update_view();
    
    game->setRound(game->getRound() + 1);
}

void View::displayEnemies() {
    for(size_t i=0; i<tileViewPath.size(); ++i) {
        TileView *tileView = tileViewPath.at(i);
        PathTile * tile = game->getPathTile(tileView->col, tileView->row);
        string enemyType = "";
        string stats = "";
        int basicCount = 0;
        int regenerativeCount = 0;
        int invisibleCount = 0;
        for (Enemy* enemy: tile->getEnemies()) {
            switch(enemy->getType()) {
                case 'B': 
                    basicCount++;
                    enemyType = "Basic";
                    break;
                case 'R': 
                    regenerativeCount++;
                    enemyType = "Regenerative";
                    break;
                default: 
                    invisibleCount++;
                    enemyType = "Invisible";
                    break;
            }
            stats += enemyType + " Enemy with " + to_string(enemy->getHP()) + " hp (" + (enemy->getFrozen() > 0 ? "frozen" : "not frozen") + " & " + (enemy->isTargetable() ? "targetable" : "not targetable") + ")\n";
        }
        tileView->label.set_tooltip_text(stats);
        if(tile->getEnemies().size() == 0 && tileView->get_style_context()->has_class("path_tile_print")) {
            tileView->get_style_context()->remove_class("path_tile_print");
        }
        if(tile->getEnemies().size() > 0) {
            tileView->get_style_context()->add_class("path_tile_print");
        }
    }        
}

void View::displayPopup(string title, string secondary, bool isError) {
    Gtk::MessageDialog dialog(*this, title, false, isError ? Gtk::MESSAGE_ERROR : Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
    dialog.set_secondary_text(secondary);
    dialog.run();
}