#ifndef OASENCRAWLER_ENEMY_H
#define OASENCRAWLER_ENEMY_H

class Enemy {
public:
    int x, y;
    int health = 1;

    Enemy(int x, int y) : x(x), y(y) {}
};

#endif //OASENCRAWLER_ENEMY_H
