#ifndef STATE_H_
#define STATE_H_

#include "./../enemy/enemy.h"
#include "./../map/map.h"
#include "./../shop/shop.h"
#include "./../tower/tower.h"
#include "./../spawner/spawner.h"
#include "./../view/view.h"

#include <stack>
#include <vector>


struct StateImpl {
    long long money;
    int hp, round;

    Shop* shop;
    Map* map;
    Spawner* spawner;

    std::vector<Tower *> towers;
    std::vector<Enemy *> enemies;
    std::stack<Enemy *> pendingEnemies;
    
    public:
        StateImpl();
        ~StateImpl();
};

class State {
	friend class View;

    private:
        StateImpl* p;

        bool surviveDmg(int);
        void incrementMoney(int);

        void addTower(Tower *);
        void addEnemy(Enemy *);
        void removeEnemy(Enemy *);
        void removeTower(Tower *);


        int constructEnemies();

        int totalHP(std::vector<Enemy *>);

        bool moveEnemies(int, int);

        bool preFrame(int, int);

        void processFrame();
        void postFrame();

        void getMoneyTowerIncome();
        void getRoundIncome();
        void updateState(int, int, double);

    public:
        static const int MAX_ROUND = 50;
        static const int MAX_LIVES = 100;
        static const int MIN_LIVES = 0;

        bool sellTower(int, int);

        State();
        ~State();

        void displayMap();
        bool buyTower(char, int, int);
        bool upgradeTower(int, int);

        void startRound();

        int getHp();
        long long getMoney();
        int getRound();
		Map* getMap();
};

#endif