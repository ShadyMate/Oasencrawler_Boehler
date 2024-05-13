#include "../../Headers/include/player.h"
#include <iostream>

void Player::printStats() {
    std::cout << "Leben: " << health << std::endl;
    std::cout << "Relikte: " << relicPoints << std::endl;
    std::cout << "Staerke: " << strength << std::endl;
    std::cout << "Intelligenz: " << intelligence << std::endl;
    std::cout << "Geschicklichkeit: " << dexterity << std::endl;
}
std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player("
       << "x: " << player.x
       << ", y: " << player.y
       << ", health: " << player.health
       << ", relicPoints: " << player.relicPoints
       << ", strength: " << player.strength
       << ", intelligence: " << player.intelligence
       << ", dexterity: " << player.dexterity
       << ")";
    return os;
}