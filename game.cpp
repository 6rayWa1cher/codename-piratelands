#include "game.h"
#include "item.h"
#include <algorithm>

Game::Game(QString name) {
    _world = std::make_shared<World>();
    _hero = std::make_shared<Hero>(name, _world);
    _shop = std::make_shared<Shop>(this);
    _shop->addItem(
                std::make_shared<ShipCannons>("C1", "Basic cannons", 18, 25, 1, 0)
                );
    _shop->addItem(
                std::make_shared<ShipHull>("H1", "Basic hull", 30, 0, 1, 0)
                );
}
