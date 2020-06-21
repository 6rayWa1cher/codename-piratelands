#include "game.h"
#include "item.h"
#include <algorithm>

Game::Game(QString name) {
    _world = std::make_shared<World>(nullptr);
    _hero = std::make_shared<Hero>(name, _world);
    _shop = std::make_shared<Shop>(this);
    _shop->addItem(
                std::make_shared<ShipCannons>("C1", "Basic cannons", 18, 25, 1, 0)
                );
    _shop->addItem(
                std::make_shared<ShipHull>("H1", "Basic hull", 30, 0, 1, 0)
                );
    _shop->addItem(
                std::make_shared<ShipCannons>("C2", "UnBasic cannons", 321, 255, 1, 0)
                );
    _shop->addItem(
                std::make_shared<ShipHull>("H2", "UnBasic hull", 12, 14, 1, 0)
                );
    _hero->addItem(
                std::make_shared<ShipHull>("NormalName1", "Basic1 hull", 301, 0, 1, 0)
                );
    _hero->addItem(
                std::make_shared<ShipHull>("qwqe123124", "Basic2 hull", 302, 0, 1, 0)
                );
    _hero->addItem(
                std::make_shared<ShipHull>("asdzxfgcgvjhi", "Basic3 hull", 303, 0, 1, 0)
                );
}
