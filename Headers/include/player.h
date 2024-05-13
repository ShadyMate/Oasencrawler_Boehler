#ifndef PLAYER_H
#define PLAYER_H
#include <random>
#include <string>
#include "item.h"
#include <iostream>

class Player {
public:
    int x, y;
    int health = 5;
    int relicPoints = 0;
    int strength, intelligence, dexterity;
    std::string nextTestWithoutRoll;
    std::vector<Item> items;

    Player(int x, int y) : x(x), y(y) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 5);
        strength = dis(gen);
        intelligence = dis(gen);
        dexterity = dis(gen);
    }

    void printStats();
    void printItems() { // Neue Methode hinzufügen
        std::cout << "Deine Items:\n";
        for (const Item& item : items) {
            std::cout << "- " << item.name << std::endl;
        }
    }
    bool operator==(const Player& other) const {
        return x == other.x && y == other.y && health == other.health && relicPoints == other.relicPoints
               && strength == other.strength && intelligence == other.intelligence && dexterity == other.dexterity;
    }

    // Overload the '!=' operator
    bool operator!=(const Player& other) const {
        return !(*this == other);
    }

    // Overload the '<' operator based on health
    bool operator<(const Player& other) const {
        return health < other.health;
    }
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

#endif
