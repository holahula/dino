#ifndef VIEW_H
#define VIEW_H

#include <memory>
#include <utility>
#include <string>
#include <gtkmm.h>

// forward declaration
class TileView;
class Tower;
class Enemy;
class State;

class View : public Gtk::Window {
    std::unique_ptr<State> game;
    TileView *selected_tileView;
    Tower *selected_tower;
    // targets for drag and drop
    std::vector<Gtk::TargetEntry> listTargets;
    // std::vector<std::vector<TileView *> > tileViewGrid;
    std::vector<TileView *> tileViewPath;

    int frame;
    bool status;
    int size;
    int hpStartRound;
    int totalEnemyHP;
    bool roundDone;

 protected:
    Gtk::Grid m_grid, tiles;
    Gtk::ButtonBox box_menu;
    Gtk::ButtonBox box_round;
    Gtk::ButtonBox box_shop;
    Gtk::ButtonBox box_info;
    Gtk::Frame frame_menu;
    Gtk::Frame frame_shop;
    Gtk::Frame frame_info;
    Gtk::Button m_button_round;
    Gtk::Button m_button_next;
    Gtk::Button m_button_skip;
    Gtk::Button m_button_buy_damage_tower;
    Gtk::Button m_button_buy_freeze_tower;
    Gtk::Button m_button_buy_money_tower;
    Gtk::Button m_button_upgrade_tower;
    Gtk::Button m_button_sell_tower;
    Gtk::Label m_label_damage_tower;
    Gtk::Label m_label_freeze_tower;
    Gtk::Label m_label_money_tower;
    Gtk::Label m_label_user_spec;
    Gtk::Label m_label_tower_spec;
    
    void on_button_round_clicked();
    void on_button_next_clicked();
    void on_button_skip_clicked();
    void on_button_upgrade_tower_clicked();
    void on_button_sell_tower_clicked();
    bool on_tile_clicked(GdkEventButton *, TileView *);

    void on_button_drag_data_get(const Glib::RefPtr<Gdk::DragContext>& context, Gtk::SelectionData& selection_data, guint info, guint time, char tower);
    void on_label_drop_drag_data_received(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time, TileView *tileView);

    void update_view();
    void update_tooltip();
    void update_info();
    void update_selected_tileView(TileView *);
    void update_selected_tower();
    std::pair<std::string, std::string> getTowerFullType(char, bool);

    void startRound();
    void nextStep();
    void updateState(int, double);
    void startNewGame(bool, bool);
    void displayEnemies();
    void displayPopup(std::string, std::string, bool);

 public:
    View(bool, bool);
    virtual ~View();
};

#endif
