#ifndef HERO_H
#define HERO_H
#include <QObject>
#include <memory>
#include "ship.h"
#include "world.h"

class Item;

enum class ItemType;

class Hero : public QObject {
    Q_OBJECT

public:
    Ship* ship;
    QString _name;
    int _health;
    int _money;
    std::shared_ptr<World> _world;
    QMap<ItemType, QList<std::shared_ptr<Item>>> inventory;
    int _currentRoom;

public:
    Hero(QString name, std::shared_ptr<World> world);
    void move(Direction dir);
signals:
    void hero_moved(int room);
};

#endif // HERO_H
