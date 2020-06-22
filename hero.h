#ifndef HERO_H
#define HERO_H
#include <QObject>
#include <memory>
#include "world.h"

class Item;
class ShipCannons;
class ShipHull;
class ShipSail;

enum class ItemType;

class Hero : public QObject {
    Q_OBJECT

private:
    QString _name;
    uint16_t _health;
    uint32_t _money;
    std::shared_ptr<World> _world;
    QMap<ItemType, QList<std::shared_ptr<Item>>> _inventory;
    uint16_t _currentRoom;

    std::shared_ptr<ShipCannons> _cannons;
    std::shared_ptr<ShipHull> _hull;
    std::shared_ptr<ShipSail> _sail;

public:
    Hero(QString name, std::shared_ptr<World> world);
    void move(Direction dir);
    void addItem(std::shared_ptr<Item> item);
    void removeItem(std::shared_ptr<Item> item);
    bool changeMoney(int delta);
    void equipCannons(std::shared_ptr<ShipCannons> cannons);
    void equipHull(std::shared_ptr<ShipHull> hull);
    void equipSail(std::shared_ptr<ShipSail> sail);

    QString name() const;

    uint16_t health() const;

    uint32_t money() const;

    std::shared_ptr<ShipCannons> cannons();

    std::shared_ptr<ShipHull> hull();

    std::shared_ptr<ShipSail> sail();

    QMap<ItemType, QList<std::shared_ptr<Item> > > inventory() const;

    uint16_t currentRoom() const;

signals:
    void hero_moved(int room);
    void money_changed(int money);
    void inventory_changed(const QMap<ItemType, QList<std::shared_ptr<Item>>>& inventory);
    void cannons_changed(std::shared_ptr<ShipCannons> cannons);
    void hull_changed(std::shared_ptr<ShipHull> hull);
    void sail_changed(std::shared_ptr<ShipSail> sail);
};

#endif // HERO_H
