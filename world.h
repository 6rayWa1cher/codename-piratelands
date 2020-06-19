#ifndef WORLD_H
#define WORLD_H

#include <QMap>

enum class Direction {
    North = 0,
    East,
    South,
    West
};

class Room {
public:
    QString _name;
    QString _description;

    QMap<Direction, int> _neighbourRooms;

    Room(QString roomName, QString roomDescr, int n, int e, int s, int w);
};

class World {
private:
    QVector<Room> _rooms;
public:
    World();
    Room operator [](int index) const;
    void init();
};

#endif // WORLD_H
