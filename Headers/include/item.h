#ifndef ITEM_H
#define ITEM_H
#include <string>

class Item {
public:
    std::string name;
    std::string effect;

    Item(std::string name, std::string effect) : name(name), effect(effect) {}
};

#endif

