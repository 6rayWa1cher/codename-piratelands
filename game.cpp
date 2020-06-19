#include "game.h"

Game::Game(QString name) {
    _world = std::make_shared<World>();
    _hero = std::make_shared<Hero>(name, _world);
}
