#include "MenuManager.h"

void MenuManager::setMenuItems(const Menu::menuItem* _items, uint8_t _count){
  items = _items;
  itemCount = _count;
  selectedIndex = 0;
}

void MenuManager::next(){
  selectedIndex = (selectedIndex + 1) % itemCount;
  changed = true;
}

void MenuManager::prev(){
  selectedIndex = (selectedIndex - 1 + itemCount) % itemCount;
  changed = true;
}

void MenuManager::select(){
  if(items[selectedIndex].action){
    items[selectedIndex].action();
  }
}

bool MenuManager::wasMenuChanged(){
  if(changed){
    changed = false;
    return true;
  }
  return false;
}