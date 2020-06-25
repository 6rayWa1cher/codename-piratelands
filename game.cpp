#include "game.h"
#include "item.h"
#include <algorithm>
#include "iostream"

Game::Game(QString name) {
    if(name.isEmpty()) name = "Долговязый Джонни";
    _world = std::make_shared<World>(nullptr);
    _hero = std::make_shared<Hero>(name, _world);
    _shop = std::make_shared<Shop>(this);
    _battle = std::make_shared<Battle>(this, this);
    QObject::connect(&*_hero, SIGNAL(hero_moved(int)), this, SLOT(heroMoved(int)));
    for(size_t i = 2; i < items.size(); i++) {
        _shop->addItem(items[i]);
    }
}

void Game::heroMoved(int index) {
    _world->heroMoved(_hero, index);    
}

