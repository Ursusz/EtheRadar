#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <stdint.h>
#include "Menu.h"

class MenuManager {
public:
  const Menu::menuItem* items;
  uint8_t itemCount;
  int8_t selectedIndex = 0;

  bool changed = true;
  void setMenuItems(const Menu::menuItem* _items, uint8_t _count);
  void next();
  void prev();
  void select();
  bool wasMenuChanged();
};

#endif
