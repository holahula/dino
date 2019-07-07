#include <iostream>
#include <sstream>
#include <string>
#include "models/enemy/enemy.h"
#include "models/state/state.h"
#include "models/tower/tower.h"
#include "models/map/map.h"
using namespace std;

int main() {
    stringstream ss;
    State state;

    char cmd, cmd2;
    int row, col;

    while (cin >> cmd && cmd != 'q') {
        switch (cmd) {
            // Start a new game
            case 'n':
                break;
            // Buy a tower
            case 'b':
                cin >> cmd2 >> row >> col;
                switch (cmd2) {
                    // damage tower
                    case 'd':
                        break;
                    // freeze tower
                    case 'f':
                        break;
                    // money tower
                    case 'm':
                        break;
                }
                break;
                // Next turn
            case 't':
                break;
            default:
                cout << "Invalid command: " << cmd << endl;
                break;
        }
    }
}
