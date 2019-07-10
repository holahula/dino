#ifndef GTKMM_EXAMPLE_VIEW_H
#define GTKMM_EXAMPLE_VIEW_H

#include "./../state/state.h"

#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm.h>

class View : public Gtk::Window {
  State game;

 public:
  View();
  virtual ~View();
  int x = 0;
  int y = 0;

 protected:
  // Signal handlers:
  void on_button_drag_data_get(
      const Glib::RefPtr<Gdk::DragContext>& context,
      Gtk::SelectionData& selection_data, guint info, guint time);
  void on_label_drop_drag_data_received(
      const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
      const Gtk::SelectionData& selection_data, guint info, guint time);
  void on_button_start_clicked();
  void on_button_print_clicked();
  void on_button_round_clicked();
  void on_button_buy_damage_tower_clicked();
  void on_button_buy_freeze_tower_clicked();
  void on_button_buy_money_tower_clicked();

  // Child widgets:
  Gtk::Grid m_grid;
  Gtk::Grid tiles;
  Gtk::Grid panel;
  Gtk::Button m_button_start;
  Gtk::Button m_button_print;
  Gtk::Button m_button_round;
  Gtk::Button m_button_buy_damage_tower;
  Gtk::Button m_button_buy_freeze_tower;
  Gtk::Button m_button_buy_money_tower;
};

#endif
