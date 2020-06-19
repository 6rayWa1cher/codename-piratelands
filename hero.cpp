#include "hero.h"

Hero::Hero(QString name, std::shared_ptr<World> world)
    : _name(name), _world(world) {
    _money = 0;
    _health = 100;
    _currentRoom = 0;
}

void Hero::move(Direction dir) {
    _currentRoom = (*_world)[_currentRoom]._neighbourRooms[dir];
    emit hero_moved(_currentRoom);
}
