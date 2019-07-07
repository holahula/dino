#include "models/enemy/enemy.h"
#include "models/state/state.h"
#include "models/tower/tower.h"
#include "models/map/map.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    stringstream ss;
    State * game;

    char cmd, cmd2;
    int x, y;

    cout << "welcome to beans, beans, beans" << endl;
    while (cin >> cmd && cmd != 'q') {
        switch (cmd) {
            // Start a new game
            case 'n':
                game = new State();
                cout << "game created" << endl;
                break;
            // Buy a tower
            case 'b':
                cin >> cmd2 >> x >> y;
                switch (cmd2) {
                    // damage tower
                    case 'd':
                        if(!game->buyTower(cmd2, x, y)){
                            cout << "Invalid Purchase! Damage tower @ (" << x << ", " << y << ") could not be bought" << endl;
                        } else {
                            cout << "Damage tower purchase successful!" << endl;
                        }
                        break;
                    // freeze tower
                    case 'f':
                        if(!game->buyTower(cmd2, x, y)){
                            cout << "Invalid Purchase! Freeze tower @ (" << x << ", " << y << ") could not be bought" << endl;
                        } else {
                            cout << "Freeze tower purchase successful!" << endl;
                        }
                        break;
                    // money tower
                    case 'm':
                        if(!game->buyTower(cmd2, x, y)){
                            cout << "Invalid Purchase! Money tower @ (" << x << ", " << y << ") could not be bought" << endl;
                        } else {
                            cout << "Money tower purchase successful!" << endl;
                        }
                        break;
                }
                break;
            // Upgrade a tower
            case 'u':
                cin >> x >> y;
                if(!game->upgradeTower(x, y)){
                    cout << "Invalid Upgrade! Tower @ (" << x << ", " << y << ") could not be upgraded" << endl;
                } else {
                    cout << "Upgrade successful!" << endl;
                }
                break;
            // Start Next round
            case 'r':
                game->startRound();
                break;

            case 'p':
                game->displayMap();
                break;
            default:
                cout << "Invalid command: " << cmd << endl;
                break;
        }
    }
}
