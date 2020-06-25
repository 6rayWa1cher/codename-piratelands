#include "hero.h"
#include "item.h"
#include <iostream>

QString Enemy::name() const {
    return _name;
}

uint16_t Enemy::health() const {
    return _health;
}

uint16_t Enemy::maxHealth() const {
    return _maxHealth;
}

uint32_t Enemy::money() const {
    return _money;
}

std::shared_ptr<ShipCannons> Enemy::cannons() {
    return _cannons;
}

std::shared_ptr<ShipHull> Enemy::hull() {
    return _hull;
}

std::shared_ptr<ShipSail> Enemy::sail() {
    return _sail;
}

const QMap<ItemType, QList<std::shared_ptr<Item> > >& Hero::inventory() const {
    return _inventory;
}

uint16_t Hero::currentRoom() const {
    return _currentRoom;
}

std::shared_ptr<ShipBoardingTeam> Enemy::team() {
    return _team;
}

Enemy::Enemy(QString name, std::shared_ptr<World> world, uint16_t health, uint32_t money, std::shared_ptr<ShipBoardingTeam> team,
             std::shared_ptr<ShipCannons> cannons, std::shared_ptr<ShipHull> hull,
             std::shared_ptr<ShipSail> sail)
    : _name(name), _world(world),
      _team(team),
      _cannons(cannons),
      _hull(hull),
      _sail(sail),
      _health(health),
      _maxHealth(health),
      _money(money){

}

Hero::Hero(QString name, std::shared_ptr<World> world) :
    Enemy(name, world, 150, 0,
          std::dynamic_pointer_cast<ShipBoardingTeam>(items[10]),
          std::dynamic_pointer_cast<ShipCannons>(items[8]),
          std::dynamic_pointer_cast<ShipHull>(items[14]),
          std::dynamic_pointer_cast<ShipSail>(items[16])
          )
{
    _currentRoom = 15;
    for (int i = 0; i < int(LAST_ITEM_TYPE); ++i) {
        _inventory[ItemType(i)] = QList<std::shared_ptr<Item>>();
    }
    equipHull(_hull);
    equipSail(_sail);
    equipTeam(_team);
    equipCannons(_cannons);
    setHealth(_maxHealth);
}

void Hero::move(Direction dir) {
    _currentRoom = (*_world)[_currentRoom]->_neighbourRooms[dir];
    emit hero_moved(_currentRoom);
}

void Hero::addItem(std::shared_ptr<Item> item) {
    auto& itemGroupInShop = _inventory[item->getType()];
    int i;
    bool equals = false;
    switch (int(item->getType())) {
    case (int) ItemType::SHIP_BOARDING_TEAM:
        equals = *_team == *item;
        break;
    case (int) ItemType::SHIP_CANNONS:
        equals = *_cannons == *item;
        break;
    case (int) ItemType::SHIP_HULL:
        equals = *_hull == *item;
        break;
    case (int) ItemType::SHIP_SAIL:
        equals = *_sail == *item;
        break;
    default:
        equals = false;
    }
    if (equals) {
        throw UniqueItemException();
    }
    for(i = 0; i < itemGroupInShop.size(); i++) {
        if(itemGroupInShop[i]->name == item->name &&
           itemGroupInShop[i]->description == item->description &&
           itemGroupInShop[i]->price == item->price) break;
    }
    if (i == itemGroupInShop.size()) {
        itemGroupInShop.push_back(item);
    } else {
        if(item->getType() == ItemType::SHIP_CONSUMABLE) {
            auto newItem = item->changeAmount(itemGroupInShop[i]->amount);
            this->removeItem(itemGroupInShop[i]);
            itemGroupInShop.append(newItem);
        }
        else {
//            size_t j;
//            for(j = 3; j < items.size(); j++) {
//                if(items[j]->name == item->name) break;
//            }
//            auto newItem = items[j];
//            itemGroupInShop.append(newItem);
            throw UniqueItemException();
        }
    }
    emit inventory_changed(_inventory);
}

void Hero::removeItem(std::shared_ptr<Item> item) {
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

void Hero::equipTeam(std::shared_ptr<ShipBoardingTeam> team) {
    _team = team;
    emit team_changed(team);
}

void Hero::equipCannons(std::shared_ptr<ShipCannons> cannons) {
    _cannons = cannons;
    emit cannons_changed(cannons);
}

void Hero::equipHull(std::shared_ptr<ShipHull> hull) {
    _hull = hull;
    _maxHealth = hull->baseHealth;
    setHealth(std::min(health(), maxHealth()));
    emit hull_changed(hull);
    emit max_health_changed(_maxHealth);
    emit health_changed(_health);
}

void Hero::equipSail(std::shared_ptr<ShipSail> sail) {
    _sail = sail;
    emit sail_changed(sail);
}

void Hero::resurrect() {
    _health = _maxHealth * 0.8;
    _currentRoom = 15;
    emit health_changed(_health);
    emit hero_moved(15);
}

void Enemy::setHealth(const uint16_t &health) {
    _health = health;
    emit health_changed(_health);
}
