#ifndef SHOP_H_
#define SHOP_H_

#include <utility>
#include "../tower/tower.h"

class Shop {
 public:
  std::pair<bool, Tower> buy(Tower);
  void sell(Tower);
};

#endif