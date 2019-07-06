#include <iostream>
#include <sstream>
#include <string>
#include "enemy/enemy.h"
#include "state/state.h"
#include "tower/tower.h"
using namespace std;

int main() {
  stringstream ss;
  vector<Enemy *> enemies;
  vector<Tower *> towers;
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