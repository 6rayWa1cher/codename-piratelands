#ifndef WORLD_H
#define WORLD_H

#include <QMap>
#include <QObject>
#include <memory>

class Hero;
enum class ItemType;

class Item;
enum class Direction {
    North = 0,
    East,
    South,
    West
};

enum class EncounterType {
    LAND = 0,
    SEA
};

class World;

class Room {
protected:
    QString _name;
    QString _description;
    World* _world;
public:
    QMap<Direction, int> _neighbourRooms;

    Room(World* world, QString roomName, QString roomDescr, int n, int e, int s, int w);

    virtual void heroStepped(std::shared_ptr<Hero> hero);
    virtual void successfulEncount();
    QString name() const;
    QString description() const;
    virtual bool isShopAvailable();
};

class Sea : public Room {
public:
    Sea(World* world, int n, int e, int s, int w);
};

class Island : public Room {
private:
    std::function<void(Island (*), std::shared_ptr<Hero>)> _heroStepped;
    std::function<void(Island (*))> _successfulEncount;
public:
    bool isCleared;
    Island(World* world, QString roomName, QString roomDescr, int n, int e, int s, int w);
    Island(World* world, QString roomName, QString roomDescr,
           int n, int e, int s, int w, std::function<void(Island (*), std::shared_ptr<Hero>)> heroStepped,
           std::function<void(Island (*))> successfulEncount);
    virtual void heroStepped(std::shared_ptr<Hero> hero) override;
    virtual void successfulEncount() override;
    virtual bool isShopAvailable() override;
};

class E1Island : public Room {
private:
    bool _isMapCollected = false;
    bool _isOver = false;
public:
    E1Island(World* world, int n, int e, int s, int w);
    virtual void heroStepped(std::shared_ptr<Hero> hero) override;
    virtual void successfulEncount() override;
private:
    void updateInfo();
};

class World : public QObject {
    Q_OBJECT
private:
    QVector<std::shared_ptr<Room>> _rooms;
public:
    World(QObject* parent);
    std::shared_ptr<Room> operator [](int index) const;
    void init();
    void sendEncounter(EncounterType type, std::shared_ptr<Hero> enemy);
signals:
    void worldChanged();
    void encounter(EncounterType type, std::shared_ptr<Hero> enemy);
public slots:
    void heroInventoryChanged(QMap<ItemType, QList<std::shared_ptr<Item>>> map);
    void heroMoved(std::shared_ptr<Hero> hero, int index);
    void encounterSuccessful(int index);
};

#endif // WORLD_H
