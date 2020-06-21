#include "hero.h"
#include "item.h"
#include <iostream>

QString Hero::name() const
{
    return _name;
}

uint16_t Hero::health() const
{
    return _health;
}

uint32_t Hero::money() const
{
    return _money;
}

std::shared_ptr<ShipCannons> Hero::cannons() {
    return _cannons;
}

std::shared_ptr<ShipHull> Hero::hull() {
    return _hull;
}

std::shared_ptr<ShipSail> Hero::sail() {
    return _sail;
}

QMap<ItemType, QList<std::shared_ptr<Item> > > Hero::inventory() const
{
    return _inventory;
}

uint16_t Hero::currentRoom() const
{
    return _currentRoom;
}

Hero::Hero(QString name, std::shared_ptr<World> world)
    : _name(name), _world(world) {
    _money = 0;
    _health = 100;
    _currentRoom = 15;
    for (int i = 0; i < int(LAST_ITEM_TYPE); ++i) {
        _inventory[ItemType(i)] = QList<std::shared_ptr<Item>>();
    }
}

void Hero::move(Direction dir) {
    _currentRoom = (*_world)[_currentRoom]->_neighbourRooms[dir];
    emit hero_moved(_currentRoom);
}

void Hero::addItem(std::shared_ptr<Item> item) {
    auto& itemGroupInShop = _inventory[item->getType()];
    if (!itemGroupInShop.contains(item)) {
        itemGroupInShop.push_back(item);
    } else {
        auto newItem = item->changeAmount(item->amount);
        itemGroupInShop.replace(itemGroupInShop.indexOf(item), newItem);
    }
    emit inventory_changed(_inventory);
}

void Hero::removeItem(std::shared_ptr<Item> item)
{
    auto& itemGroupInShop = _inventory[item->getType()];
    int i = 0;
    for(i = 0; i < itemGroupInShop.size(); i++) {
        if(itemGroupInShop[i]->name == item->name &&
           itemGroupInShop[i]->description == item->description &&
           itemGroupInShop[i]->price == item->price) break;
    }
    if (i == itemGroupInShop.size()) {
        std::cerr << "Can't remove item which isn't presented" << std::endl;
        return;
    }

    auto newItem = item->changeAmount(-int64_t(item->amount));
    if (newItem) {
        itemGroupInShop.replace(itemGroupInShop.indexOf(item), newItem);
    } else {
        itemGroupInShop.removeAt(i);
    }
    emit inventory_changed(_inventory);
}

bool Hero::changeMoney(int delta) {
    if (delta < 0 && static_cast<uint32_t>(-delta) > _money) {
        return false;
    }
    _money += delta;
    emit money_changed(_money);
    return true;
}

void Hero::equipCannons(std::shared_ptr<ShipCannons> cannons) {
    _cannons = cannons;
    emit cannons_changed(cannons);
}

void Hero::equipHull(std::shared_ptr<ShipHull> hull) {
    _hull = hull;
    emit hull_changed(hull);
}

void Hero::equipSail(std::shared_ptr<ShipSail> sail) {
    _sail = sail;
    emit sail_changed(sail);
}
