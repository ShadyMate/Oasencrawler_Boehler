#ifndef WORLD_H
#define WORLD_H

#include "player.h"
#include "enemy.h"
#include "item.h"
#include <array>
#include <random>
#include <vector>

class World {
public:
    std::array<std::array<char, 5>, 5> grid;
    Player* player;
    Enemy* enemy;
    char prevEnemyTile = 'X';
    int difficulty = 0;
    std::vector<Item> items;

    World(Player* p, Enemy* e) : player(p), enemy(e) {
        generateWorld();
        //Items
        items.push_back(Item("Oma's Lesebrille", "intelligence"));
        items.push_back(Item("Schwert von Kas", "strength"));
        items.push_back(Item("Gefluegelte Schuhe", "dexterity"));
        items.push_back(Item("Magischer Orb", "ability"));
    }

    void generateWorld();
    void displayWorld();
    void movePlayer(char input);
    void moveEnemy();
    void findItem(char cellType);
};

#endif