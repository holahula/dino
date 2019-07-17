#include "models/enemy/enemy.h"
#include "models/map/map.h"
#include "models/state/state.h"
#include "models/tower/tower.h"
#include "models/view/view.h"

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <gtkmm.h>

using namespace std;

int graphical_interface(int argc, char *argv[], bool adaptive, bool map) {
    argc = 1;
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

	// Setup styling
    Glib::ustring cssFile = "resources/css/style.css";
    Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
    Glib::RefPtr<Gtk::StyleContext> style_context = Gtk::StyleContext::create();
 
    if(css_provider->load_from_path(cssFile)){
        Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
        style_context->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    // Instantiate window
    View view{adaptive, map};

    return app->run(view);
}

int text_interface(bool adaptive, bool map) {
    stringstream ss;
    unique_ptr<State> game(new State(adaptive, map));

    char cmd, cmd2;
    int x, y;
    string strX, strY;

    cout << "welcome to baloon tower defence!" << endl;

    while (cin >> cmd && cmd != 'q') {
        switch (cmd) {
            // Start a new game
            case 'n': {
                game = unique_ptr<State>(new State(adaptive, map));
                cout << "game created" << endl;
                break;
            }
            // Buy a tower
            case 'b': {
                cin >> cmd2 >> strX >> strY;
                x = atoi(strX.c_str());
                y = atoi(strY.c_str());
                switch (cmd2) {
                    // damage tower
                    case 'd': {
                        if (!game->buyTower('D', x, y)) {
                            cout << "Invalid Purchase! Damage tower @ (" << x << ", " << y << ") could not be bought" << endl;
                        } else {
                            cout << "Damage tower purchase successful!" << endl;
                        }
                        break;
                    }
                    // freeze tower
                    case 'f': {
                        if (!game->buyTower('F', x, y)) {
                            cout << "Invalid Purchase! Freeze tower @ (" << x << ", " << y << ") could not be bought" << endl;
                        } else {
                            cout << "Freeze tower purchase successful!" << endl;
                        }
                        break;
                    }
                    // money tower
                    case 'm': {
                        if (!game->buyTower('M', x, y)) {
                            cout << "Invalid Purchase! Money tower @ (" << x << ", " << y << ") could not be bought" << endl;
                        } else {
                            cout << "Money tower purchase successful!" << endl;
                        }
                        break;
                    }
                    default: {
                        cout << "That is not a valid tower!" << endl;
                    }
                }
                break;
            }
            // Upgrade a tower
            case 'u': {
                cin >> strX >> strY;
                x = atoi(strX.c_str());
                y = atoi(strY.c_str());
                if (!game->upgradeTower(x, y)) {
                    cout << "Invalid Upgrade! Tower @ (" << x << ", " << y << ") could not be upgraded" << endl;
                } else {
                    cout << "Upgrade successful!" << endl;
                }
                break;
            }
            // Sell a tower
            case 's': {
                cin >> strX >> strY;
                x = atoi(strX.c_str());
                y = atoi(strY.c_str());
                if (!game->sellTower(x, y)) {
                    cout << "Invalid Sell Operation! Tower @ (" << x << ", " << y << ") could not be sold" << endl;
                } else {
                    cout << "Sold successfully!" << endl;
                }
                break;
            }
            // Get total money
            case 'm': {
                cout << "You currently have: $" << game->getMoney() << endl;
                break;
            }
            // Start Next round
            case 'r': {
                game->startRound();
                break;
            }
            // Print map
            case 'p': {
                game->displayMap();
                break;
            }
            default: {
                cout << "Invalid command: " << cmd << endl;
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    bool ui = true, adaptive = false, map = false;

    for(int i=1; i<argc; ++i) {
        string cmd = string(argv[i]);
        
        if(cmd == "--extras") adaptive = true;
        else if (cmd == "--map") map = true;
        else if (cmd == "--text") ui = false;
        else {
            cout << "Only acceptable flags are --extras, --map, --text" << endl;
            cout << "   --extras -> adds adaptive level generation" << endl;
            cout << "   --map       -> adds unique path generation" << endl;
            cout << "   --text      -> utilizes the text interface" << endl;
            exit(1);
        }
    }

    if(ui) return graphical_interface(argc, argv, adaptive, map);
    return text_interface(adaptive, map);
}

// possible flags: 