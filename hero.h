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

private:
    Ship* _ship;
    QString _name;
    uint16_t _health;
    uint32_t _money;
    std::shared_ptr<World> _world;
    QMap<ItemType, QList<std::shared_ptr<Item>>> _inventory;
    uint16_t _currentRoom;

public:
    Hero(QString name, std::shared_ptr<World> world);
    void move(Direction dir);
    void addItem(std::shared_ptr<Item> item);
    void removeItem(std::shared_ptr<Item> item);
    bool changeMoney(int delta);
    Ship *ship() const;

    QString name() const;

    uint16_t health() const;

    uint32_t money() const;

    QMap<ItemType, QList<std::shared_ptr<Item> > > inventory() const;

    uint16_t currentRoom() const;

signals:
    void hero_moved(int room);
    void money_changed(int money);
    void inventory_changed(const QMap<ItemType, QList<std::shared_ptr<Item>>>& inventory);
};

#endif // HERO_H
