#include "../../Headers/include/world.h"
#include <iostream>
#include <random>

void World::generateWorld() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> d({4, 4, 1, 1});

    bool relicExists = false;
    for (auto &row : grid) {
        for (char &cell : row) {
            int num = d(gen);
            switch (num) {
                case 0: cell = 'X'; break;
                case 1: cell = 'G'; break;
                case 2: cell = 'B'; break;
                case 3: cell = 'R'; relicExists = true; break;
            }
        }
    }

    if (!relicExists) {
        int x = gen() % 5;
        int y = gen() % 5;
        grid[x][y] = 'R';
    }

    // Spieler Position
    grid[player->x][player->y] = 'P';

    // Gegner Position, mindestens 2 Felder entfernt vom Spieler
    do {
        enemy->x = gen() % 5;
        enemy->y = gen() % 5;
    } while (abs(enemy->x - player->x) < 2 && abs(enemy->y - player->y) < 2);
    prevEnemyTile = grid[enemy->x][enemy->y];
    grid[enemy->x][enemy->y] = 'E';
}

//Funktion um Items zu finden und Stats zu verbessern
void World::findItem(char cellType) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    int chance = dis(gen);
    if ((cellType == 'B' && chance <= 3) || (cellType == 'R' && chance <= 6)) {
        std::uniform_int_distribution<> disItem(0, items.size() - 1);
        Item foundItem = items[disItem(gen)];
        std::cout << "Du hast " << foundItem.name << " gefunden!" << std::endl;
        if (foundItem.effect == "ability") {
            std::string input;
            std::cout << "Welche Faehigkeit moechtest du um 2 verbessern? (Str/Int/Dex): ";
            std::cin >> input;
            if (input == "Str") {
                player->strength += 2;
                player->printStats();
            } else if (input == "Int") {
                player->intelligence += 2;
                player->printStats();
            } else if (input == "Dex") {
                player->dexterity += 2;
                player->printStats();
            } else {
                std::cout << "Ungueltige Eingabe. Keine Verbesserung vorgenommen." << std::endl;
            }
        } else {
            player->nextTestWithoutRoll = foundItem.effect;
        }
    }
}

void World::moveEnemy() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    // Der Feind bewegt sich mit einer Wahrscheinlichkeit von 7/10
    if (dis(gen) > 3) {
        int dx = player->x - enemy->x;
        int dy = player->y - enemy->y;

        // Vorheriges Feld des Feindes wiederherstellen
        grid[enemy->x][enemy->y] = prevEnemyTile;

        // Bewege den Feind in die Richtung des Spielers
        if (abs(dx) > abs(dy)) {
            enemy->x += dx > 0 ? 1 : -1;
        } else {
            enemy->y += dy > 0 ? 1 : -1;
        }

        // Player Collision
        if (enemy->x == player->x && enemy->y == player->y) {
            player->health--;
            std::cout << "Der Gegner hat dich getroffen! Du hast jetzt " << player->health << " Lebenspunkte.\n";
            // Der Gegner despawnt
            enemy->x = -1;
            enemy->y = -1;
            prevEnemyTile = 'X';
        } else {
            // Vorheriges Tile speichern
            prevEnemyTile = grid[enemy->x][enemy->y];
            // Neues Tile wird E
            grid[enemy->x][enemy->y] = 'E';
        }
    }
}

void World::movePlayer(char input) {
    int dx = 0, dy = 0;
    switch (input) {
        case 'W': case 'w': dx = -1; break;
        case 'A': case 'a': dy = -1; break;
        case 'S': case 's': dx = 1; break;
        case 'D': case 'd': dy = 1; break;
        default: throw std::invalid_argument("Ungültige Eingabe");
    }

    int newX = player->x + dx;
    int newY = player->y + dy;

    if (newX >= 0 && newX < 5 && newY >= 0 && newY < 5) {
        // Felder werden leer wenn Spieler dort war
        grid[player->x][player->y] = 'X';

        player->x = newX;
        player->y = newY;

        char cell = grid[newX][newY];
        findItem(cell); // Item finden?
        switch (cell) {
            case 'G':
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(1, 3);
                int ability = dis(gen);
                int abilityValue;
                std::string abilityName;
                switch (ability) {
                    case 1: abilityValue = player->strength; abilityName = "strength";
                    std::cout << "Eine Strength Probe mit einer Schwierigkeit von " << 4 + difficulty << " steht an!" << std::endl;
                    break;
                    case 2: abilityValue = player->intelligence; abilityName = "intelligence";
                    std::cout << "Eine Intelligence Probe mit einer Schwierigkeit von " << 4 + difficulty << " steht an!" << std::endl;
                    break;
                    case 3: abilityValue = player->dexterity; abilityName = "dexterity";
                    std::cout << "Eine Dexterity Probe mit einer Schwierigkeit von " << 4 + difficulty << " steht an!" << std::endl;
                    break;
                }
                if (player->nextTestWithoutRoll == abilityName || abilityValue >= 4 + difficulty) {
                    std::cout << "Du hast die Gefahr ueberwunden." << std::endl;
                    if (player->nextTestWithoutRoll == abilityName) {
                        std::cout << "Du hast " << player->nextTestWithoutRoll << " verbraucht um die Gefahr zu überwinden." << std::endl;
                        player->nextTestWithoutRoll = ""; // Item verbrauchen
                    }
                } else {
                    player->health--;
                    std::cout << "Du hast einen Lebenspunkt verloren. Aktuelle Lebenspunkte: " << player->health << std::endl;
                }
            }
                break;
            case 'B':
                player->health = std::min(5, player->health + 1);
                std::cout << "Du hast einen Lebenspunkt geheilt. Aktuelle Lebenspunkte: " << player->health << std::endl;
                break;
            case 'R':
                player->relicPoints++;
                std::cout << "Du hast ein Relikt gefunden! Du hast jetzt " << player->relicPoints;
                if (player->relicPoints > 1) {
                    std::cout << " Relikte.\n";
                } else {
                    std::cout << " Relikt.\n";
                }
                grid[newX][newY] = 'X'; // Das Relikt wurde gesammelt und das Feld wird leer
                break;
        }

        // Neues Feld des Spielers wird P
        grid[player->x][player->y] = 'P';
    } else {
        throw std::out_of_range("Here be Dragons!"); // Falls der Spieler ausserhalb des Spielfelds geht
    }
    if (player->x == enemy->x && player->y == enemy->y) {
        enemy->health--;
        if (enemy->health <= 0) {
            // Wenn der Spieler den Gegner trifft dann verschwindet der Gegner
            grid[enemy->x][enemy->y] = 'X';
        }
    }
    if (player->x == enemy->x && player->y == enemy->y) {
        player->health--;
        std::cout << "Du hast den Gegner getroffen! Du hast jetzt " << player->health << " Lebenspunkte.\n";
        enemy->x = -1;
        enemy->y = -1;
        prevEnemyTile = 'X';
    }
    moveEnemy();
    //Um sicherzustellen das der Spieler immer angezeigt wird
    grid[player->x][player->y] = 'P';
}



void World::displayWorld() {
    for (const auto &row : grid) {
        for (const char &cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
}


