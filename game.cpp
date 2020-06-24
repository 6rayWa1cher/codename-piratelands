#include "game.h"
#include "item.h"
#include <algorithm>

Game::Game(QString name) {
    if(name.isEmpty()) name = "Долговязый Джонни";
    _world = std::make_shared<World>(nullptr);
    _hero = std::make_shared<Hero>(name, _world);
    _shop = std::make_shared<Shop>(this);
    _battle = std::make_shared<Battle>(this, this);
    QObject::connect(&*_hero, SIGNAL(hero_moved(int)), this, SLOT(heroMoved(int)));
    _shop->addItem(
                std::make_shared<ShipCannons>("C1", "Basic cannons", 18, 25, 1, 0)
                );
    _shop->addItem(
                std::make_shared<ShipHull>("H1", "Basic hull", 30, 0, 1, 0, 1, 0, 0)
                );
    _shop->addItem(
                std::make_shared<BombItem>("Bomb", "Bomb da enemy's s", 40, 0, 1)
                );
    _shop->addItem(
                std::make_shared<HealingItem>("Wooden plank", "Healing your health", 55, 5, 1)
                );
    _shop->addItem(
                std::make_shared<ShipCannons>("C2", "UnBasic cannons", 321, 255, 1, 0)
                );
    _shop->addItem(
                std::make_shared<ShipHull>("H2", "UnBasic hull", 12, 14, 1, 0, 1, 0, 0)
                );
    _hero->addItem(
                std::make_shared<ShipHull>("NormalName1", "Basic1 hull", 301, 0, 1, 0, 1, 0, 0)
                );
    _hero->addItem(
                std::make_shared<ShipHull>("qwqe123124", "Basic2 hull", 302, 0, 1, 0, 1, 0, 0)
                );
    _hero->addItem(
                std::make_shared<ShipHull>("asdzxfgcgvjhi", "Basic3 hull", 303, 0, 1, 0, 1, 0, 0)
                );
    _hero->addItem(std::make_shared<BombItem>()->changeAmount(100));
    _hero->addItem(std::make_shared<ShipBoardingTeam>("jipwert", "111fgasdjkpi", 100500, 0));
    _hero->addItem(std::make_shared<ShipBoardingTeam>("hioltu", "opha3rt9054y8p", 23048, 0));
    _hero->addItem(std::make_shared<HealingItem>("Metal list", "Healing your health", 120, 0, 1));
}

void Game::heroMoved(int index) {
    _world->heroMoved(_hero, index);
}
