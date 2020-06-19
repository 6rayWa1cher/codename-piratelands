#ifndef GAME_H
#define GAME_H
#include "hero.h"

class Game
{
public:
    std::shared_ptr<Hero> _hero;
    std::shared_ptr<World> _world;

    Game(QString name);
};

#endif // GAME_H
