#include "world.h"

Room::Room(QString roomName, QString roomDescr, int n, int e, int s, int w)
    : _name(roomName), _description(roomDescr){
    _neighbourRooms[Direction::North] = n;
    _neighbourRooms[Direction::East] = e;
    _neighbourRooms[Direction::South] = s;
    _neighbourRooms[Direction::West] = w;
}

World::World() {
    init();
}

Room World::operator [](int index) const {
    return _rooms[index];
}

void World::init() {
 _rooms.append(Room("Крыльцо", "Вы стоите на крыльце. Перед Вами находится совершенно загадочная дверь.", 1, -1, -1, -1 ) );
 _rooms.append(Room("Гостиная", "Вы попали в гостиную. В камине сидит страшный жирный паук.", -1, -1, 0, 2));
 _rooms.append(Room("Кабинет", "Вы стоите в кабинете. Книжный шкаф полон старинных книг.", -1, 1, -1, -1));
}
