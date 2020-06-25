#include "world.h"
#include "hero.h"
#include "item.h"


Room::Room(World* world, QString roomName, QString roomDescr, int n, int e, int s, int w)
    : _name(roomName), _description(roomDescr), _world(world) {
    _neighbourRooms[Direction::North] = n;
    _neighbourRooms[Direction::East] = e;
    _neighbourRooms[Direction::South] = s;
    _neighbourRooms[Direction::West] = w;
}
void Room::heroStepped(std::shared_ptr<Hero> /*hero*/) {

}

void Room::successfulEncount(std::shared_ptr<Hero> /*hero*/) {

}

QString Room::name() const {
    return _name;
}

QString Room::description() const {
    return _description;
}

bool Room::isShopAvailable() {
    return false;
}

Sea::Sea(World* world, int n, int e, int s, int w) :
    Room(world, "Море", "Вы на корабле где-то посреди моря", n, e, s, w) {

}

bool Island::firstTimeStep() const {
    return _firstTimeStep;
}

Island::Island(World* world, QString roomName, QString roomDescr, int n, int e, int s, int w) :
    Room(world, roomName, roomDescr, n, e, s, w), _heroStepped([](Island*, std::shared_ptr<Hero>){}),
_successfulEncount([](Island*, std::shared_ptr<Hero>){}) {

}

Island::Island(World *world, QString roomName, QString roomDescr, int n, int e, int s, int w,
               std::function<void (Island *, std::shared_ptr<Hero>)> heroStepped,
               std::function<void (Island *, std::shared_ptr<Hero>)> successfulEncount) :
    Room(world, roomName, roomDescr, n, e, s, w), _heroStepped(heroStepped), _successfulEncount(successfulEncount)
{

}

void Island::heroStepped(std::shared_ptr<Hero> hero) {
    _heroStepped(this, hero);
    _firstTimeStep = false;
}

void Island::successfulEncount(std::shared_ptr<Hero> hero) {
    _successfulEncount(this, hero);
}

bool Island::isShopAvailable() {
    return true;
}

E1Island::E1Island(World* world, int n, int e, int s, int w) : Room(world, "", "", n, e, s, w) {
    updateInfo();
}

void E1Island::heroStepped(std::shared_ptr<Hero> hero) {
    bool p1 = false, p2 = false;
    for (const auto& i : hero->inventory()[ItemType::KEY_ITEM]) {
        p1 |= i->name == "Карта часть#1";
        p2 |= i->name == "Карта часть#2";
    }
    if (p1 && p2) {
        _isMapCollected = true;
        updateInfo();
        if (_isOver) {
            return;
        } else {
            _world->sendEncounter(EncounterType::LAND, nullptr);
        }
    } else {
        _isMapCollected = false;
        updateInfo();
        return;
    }
}

void E1Island::successfulEncount(std::shared_ptr<Hero> /*hero*/) {
    _isOver = true;
    _neighbourRooms.clear();
    updateInfo();
}

void E1Island::updateInfo() {
    if (_isMapCollected && _isOver) {
        _name = "Необитаемый остров";
    } else {
        _name = "Море";
    }
    if (_isMapCollected && _isOver) {
        _description = "Вы нашли легендарный клад! Поздравляем, игра окончена!";
    } else if (_isMapCollected) {
        _description = "За вами следовал линейный корабль!";
    } else {
        _description = "Вы на корабле где-то посреди моря. Повсюду туман.";
    }
    _world->sendWorldChanged();
}

World::World(QObject* parent) : QObject(parent) {
    init();
}

std::shared_ptr<Room> World::operator [](int index) const {
    return _rooms[index];
}

void World::init() {
    _rooms.append(std::make_shared<Sea>(this, 18, 1, 6, 5)); // A1
    _rooms.append(std::make_shared<Sea>(this, 19, 2, 7, 0)); // B1
    _rooms.append(std::make_shared<Sea>(this, 20, 3, 8, 1)); // C1
    _rooms.append(std::make_shared<Sea>(this, 21, 4, 9, 2)); // D1
    _rooms.append(std::make_shared<E1Island>(this, 22, 5, 10, 3)); // E1
    _rooms.append(std::make_shared<Sea>(this, 23, 0, 11, 4)); // F1
    _rooms.append(std::make_shared<Sea>(this, 0, 7, 12, 11)); // A2
    _rooms.append(std::make_shared<Island>(this, "Остров Куба, Испания", "Лучший ром на всем архипелаге!", 1, 8, 13, 6,
                                           [](Island* island, std::shared_ptr<Hero> hero) {
                      if (island->firstTimeStep()) {
                          island->_world->grantItem(hero, std::dynamic_pointer_cast<MapPiece>(items[0]));
                      }
                  },
                  [](Island*, std::shared_ptr<Hero>) {}
    )); // B2
    _rooms.append(std::make_shared<Sea>(this, 2, 9, 14, 7)); // C2
    _rooms.append(std::make_shared<Sea>(this, 3, 10, 15, 8)); // D2
    _rooms.append(std::make_shared<Sea>(this, 4, 11, 16, 9)); // E2
    _rooms.append(std::make_shared<Sea>(this, 5, 6, 17, 10)); // F2
    _rooms.append(std::make_shared<Sea>(this, 6, 13, 18, 17)); // A3
    _rooms.append(std::make_shared<Sea>(this, 7, 14, 19, 12)); // B3
    _rooms.append(std::make_shared<Sea>(this, 8, 15, 20, 13)); // C3
    _rooms.append(std::make_shared<Island>(this, "Остров Невис, Англия",
                                           "Здесь началось ваше приключение в поисках легендарного клада Чёрной бороды...",
                                           9, 16, 21, 14)); // D3
    _rooms.append(std::make_shared<Sea>(this, 10, 17, 22, 15)); // E3
    _rooms.append(std::make_shared<Sea>(this, 11, 12, 23, 16)); // F3
    _rooms.append(std::make_shared<Island>(
                      this, "Остров Ямайка, Англия", "Один из мощнейших фортов Карибского моря",
                      12, 19, 0, 23,
                      [](Island* island, std::shared_ptr<Hero> hero) {
                        if (island->firstTimeStep()) {
                            island->_world->grantItem(hero, std::dynamic_pointer_cast<MapPiece>(items[1]));
                        }
                      },
                  [](Island*, std::shared_ptr<Hero>) {}
    )); // A4
    _rooms.append(std::make_shared<Sea>(this, 13, 20, 1, 18)); // B4
    _rooms.append(std::make_shared<Sea>(this, 14, 21, 2, 19)); // C4
    _rooms.append(std::make_shared<Sea>(this, 15, 22, 3, 20)); // D4
    _rooms.append(std::make_shared<Island>(this, "Остров Кюрасао, Голландия", "Отдаленный тихий уголок", 16, 23, 4, 21,
                                           [](Island* island, std::shared_ptr<Hero> /*hero*/) {
//                      if (island->firstTimeStep()) {
                          island->_world->sendEncounter(EncounterType::SEA, std::make_shared<Enemy> (
                          "Пират", island->_world->shared_from_this(), 120, 100,
                          std::dynamic_pointer_cast<ShipBoardingTeam>(items[10]),
                          std::dynamic_pointer_cast<ShipCannons>(items[7]),
                          std::dynamic_pointer_cast<ShipHull>(items[13]),
                          std::dynamic_pointer_cast<ShipSail>(items[16])
                          ));
//                      }
                  },
                  [](Island*, std::shared_ptr<Hero>) {}
    )); // E4
    _rooms.append(std::make_shared<Sea>(this, 17, 18, 5, 22)); // F4
}

void World::sendEncounter(EncounterType type, std::shared_ptr<Enemy> enemy) {
    emit encounter(type, enemy);
}

void World::sendWorldChanged() {
    emit worldChanged();
}

void World::grantItem(std::shared_ptr<Hero> hero, std::shared_ptr<Item> item)
{
    hero->addItem(item);
    emit loudAddItem(item);
}

void World::heroMoved(std::shared_ptr<Hero> hero, int index) {
    _rooms[index]->heroStepped(hero);
}

void World::encounterSuccessful(int index, std::shared_ptr<Hero> hero) {
    _rooms[index]->successfulEncount(hero);
}
