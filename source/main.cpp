#include "../Headers/include/player.h"
#include "../Headers/include/world.h"
#include <iostream>

/** int main() {
    Player player(0, 0);
    Enemy enemy(3, 3);
    World world(&player, &enemy);

    while (true) {
        world.generateWorld();
        world.displayWorld();
        player.printStats();
        player.printItems();

        while (player.health > 0) {
            char input;
            try {
                std::cout << "Wohin moechtest du dich bewegen? (WASD): ";
                std::cin >> input;
                world.movePlayer(input);
                world.displayWorld();
            } catch (const std::invalid_argument& e) {
                std::cout << e.what() << ". Bitte geben Sie W/A/S/D ein." << std::endl;
                continue;
            } catch (const std::out_of_range& e) {
                std::cout << e.what() << ". Bitte geben Sie eine gueltige Bewegung ein." << std::endl;
                continue;
            }

            // Relikt check
            bool allRelicsCollected = true;
            for (const auto &row : world.grid) {
                for (const char &cell : row) {
                    if (cell == 'R') {
                        allRelicsCollected = false;
                        break;
                    }
                }
                if (!allRelicsCollected) break;
            }

            // Restart falls alle Relikte gesammelt wurden
            if (allRelicsCollected) {
                std::cout << "Du hast alle Relikte gesammelt! Das Spiel wird neugestartet." << std::endl;
                player.x = 0;
                player.y = 0;
                world.difficulty++;
                break;
            }
        }

        if (player.health <= 0) {
            std::cout << "GAME OVER" << std::endl;
            break;
        }
    }

    return 0;
} **/