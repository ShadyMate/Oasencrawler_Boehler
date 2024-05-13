#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "player.h"
#include "world.h"

TEST_CASE("Player stats are displayed correctly with the Default Stats: 5 Health, 0 Relic Points, 1-5 STR/INT/DEX ", "[player]") {
    Player player(0, 0);

    std::ostringstream output;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    player.printStats();

    std::cout.rdbuf(oldCoutStreamBuf);

    REQUIRE(player.health == 5);
    REQUIRE(player.relicPoints == 0);
    REQUIRE(player.strength >= 1);
    REQUIRE(player.strength <= 5);
    REQUIRE(player.intelligence >= 1);
    REQUIRE(player.intelligence <= 5);
    REQUIRE(player.dexterity >= 1);
    REQUIRE(player.dexterity <= 5);
}
TEST_CASE("World generation creates a valid grid", "[world]") {
    World world(new Player(0, 0), new Enemy(3, 3));
    world.generateWorld();

    int relicCount = 0;
    for (const auto &row : world.grid) {
        for (const char &cell : row) {
            if (cell == 'R') {
                relicCount++;
            }
        }
    }

    REQUIRE(relicCount >= 1);
}
TEST_CASE("World generation places player and enemy correctly", "[world]") {
    World world(new Player(0, 0), new Enemy(3, 3));
    world.generateWorld();

    REQUIRE(world.grid[world.player->x][world.player->y] == 'P');
    REQUIRE(world.grid[world.enemy->x][world.enemy->y] == 'E');
}
TEST_CASE("World display outputs correct grid", "[world]") {
    World world(new Player(0, 0), new Enemy(3, 3));
    world.generateWorld();

    std::ostringstream output;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    world.displayWorld();

    std::cout.rdbuf(oldCoutStreamBuf);

    std::string expectedOutput;
    for (const auto &row : world.grid) {
        for (const char &cell : row) {
            expectedOutput += cell;
            expectedOutput += ' ';
        }
        expectedOutput += '\n';
    }

    REQUIRE(output.str() == expectedOutput);
}
TEST_CASE("Player moves correctly on valid input", "[world]") {
    World world(new Player(1, 1), new Enemy(3, 3));
    world.generateWorld();
    int initialX = world.player->x;
    int initialY = world.player->y;

    world.movePlayer('w');
    REQUIRE(world.player->x == initialX - 1);
    REQUIRE(world.player->y == initialY);

    world.movePlayer('s');
    REQUIRE(world.player->x == initialX);
    REQUIRE(world.player->y == initialY);

    world.movePlayer('a');
    REQUIRE(world.player->x == initialX);
    REQUIRE(world.player->y == initialY - 1);

    world.movePlayer('d');
    REQUIRE(world.player->x == initialX);
    REQUIRE(world.player->y == initialY);
}

TEST_CASE("Player does not move on invalid input", "[world]") {
    World world(new Player(0, 0), new Enemy(3, 3));
    world.generateWorld();
    int initialX = world.player->x;
    int initialY = world.player->y;

    REQUIRE_THROWS_AS(world.movePlayer('z'), std::invalid_argument);

    REQUIRE(world.player->x == initialX);
    REQUIRE(world.player->y == initialY);
}

TEST_CASE("Player does not move outside of grid", "[world]") {
    World world(new Player(0, 0), new Enemy(3, 3));
    world.generateWorld();
    world.player->x = 0;
    world.player->y = 0;

    REQUIRE_THROWS_AS(world.movePlayer('w'), std::out_of_range);
    REQUIRE_THROWS_AS(world.movePlayer('a'), std::out_of_range);

    REQUIRE(world.player->x == 0);
    REQUIRE(world.player->y == 0);
}

TEST_CASE("Player and enemy collide correctly", "[world]") {
    World world(new Player(0, 0), new Enemy(0, 1));
    world.generateWorld();

    // Move the enemy to the player's position
    world.enemy->x = world.player->x;
    world.enemy->y = world.player->y+1;

    world.moveEnemy();

    REQUIRE(world.player->health == 4);
    REQUIRE(world.enemy->x == -1);
    REQUIRE(world.enemy->y == -1);
    REQUIRE(world.grid[world.player->x][world.player->y] == 'P');
}


TEST_CASE("Enemy moves correctly when not colliding with player", "[world]") {
    World world(new Player(0, 0), new Enemy(3, 3));
    world.generateWorld();
    int initialEnemyX = world.enemy->x;
    int initialEnemyY = world.enemy->y;

    world.moveEnemy();

    REQUIRE((world.enemy->x != initialEnemyX || world.enemy->y != initialEnemyY));
    REQUIRE(world.grid[world.enemy->x][world.enemy->y] == 'E');
}
TEST_CASE("Player operator overloads work correctly", "[player]") {
    Player player1(0, 0);
    player1.strength = 3;
    player1.intelligence = 4;
    player1.dexterity = 5;

    Player player2(0, 0);
    player2.strength = 3;
    player2.intelligence = 4;
    player2.dexterity = 5;

    Player player3(1, 1);
    player3.strength = 3;
    player3.intelligence = 4;
    player3.dexterity = 5;

    // Test '==' operator
    REQUIRE(player1 == player2);
    REQUIRE_FALSE(player1 == player3);

    // Test '!=' operator
    REQUIRE_FALSE(player1 != player2);
    REQUIRE(player1 != player3);

    // Test '<' operator
    player1.health = 3;
    player2.health = 4;
    REQUIRE(player1 < player2);
    REQUIRE_FALSE(player2 < player1);
}