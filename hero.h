#ifndef HERO_H
#define HERO_H
#include <QObject>
#include <memory>
#include "world.h"

class Item;
class ShipCannons;
class ShipHull;
class ShipSail;
class ShipBoardingTeam;

enum class ItemType;

class Enemy : public QObject {
    Q_OBJECT
protected:
    QString _name;
    uint16_t _health;
    uint16_t _maxHealth;
    uint32_t _money;
    std::shared_ptr<World> _world;
    std::shared_ptr<ShipBoardingTeam> _team;
    std::shared_ptr<ShipCannons> _cannons;
    std::shared_ptr<ShipHull> _hull;
    std::shared_ptr<ShipSail> _sail;

public:
    Enemy(QString name, std::shared_ptr<World> world, uint16_t health, uint32_t money, std::shared_ptr<ShipBoardingTeam> team = nullptr,
          std::shared_ptr<ShipCannons> cannons = nullptr, std::shared_ptr<ShipHull> hull = nullptr,
          std::shared_ptr<ShipSail> sail = nullptr);

    QString name() const;

    uint16_t health() const;

    uint16_t maxHealth() const;

    uint32_t money() const;

    std::shared_ptr<ShipBoardingTeam> team();

    std::shared_ptr<ShipCannons> cannons();

    std::shared_ptr<ShipHull> hull();

    std::shared_ptr<ShipSail> sail();

    void setHealth(const uint16_t &health);
};

class Hero : public Enemy {
    Q_OBJECT

private:
    QMap<ItemType, QList<std::shared_ptr<Item>>> _inventory;
    uint16_t _currentRoom;
public:
    Hero(QString name, std::shared_ptr<World> world);
    void move(Direction dir);
    void addItem(std::shared_ptr<Item> item);
    void removeItem(std::shared_ptr<Item> item);
    bool changeMoney(int delta);
    void equipTeam(std::shared_ptr<ShipBoardingTeam> team);
    void equipCannons(std::shared_ptr<ShipCannons> cannons);
    void equipHull(std::shared_ptr<ShipHull> hull);
    void equipSail(std::shared_ptr<ShipSail> sail);

    QMap<ItemType, QList<std::shared_ptr<Item> > > inventory() const;

    uint16_t currentRoom() const;

    void setHeroHealth(const uint16_t &health);

signals:
    void hero_moved(int room);
    void money_changed(int money);
    void inventory_changed(const QMap<ItemType, QList<std::shared_ptr<Item>>>& inventory);
    void team_changed(std::shared_ptr<ShipBoardingTeam> team);
    void cannons_changed(std::shared_ptr<ShipCannons> cannons);
    void hull_changed(std::shared_ptr<ShipHull> hull);
    void sail_changed(std::shared_ptr<ShipSail> sail);
    void hero_health_changed(uint16_t health);
    void max_health_changed(uint16_t health);
};

#endif // HERO_H
