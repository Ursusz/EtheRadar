#pragma once

#include <string>
#include <functional>

class Menu {
public:
  typedef std::function<void()> menuAction;
  struct menuItem{
    std::string label;
    menuAction action;
  };
};