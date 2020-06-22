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
public:
    QMap<Direction, int> _neighbourRooms;
    World* _world;

    Room(World* world, QString roomName, QString roomDescr, int n, int e, int s, int w);

    virtual void heroStepped(std::shared_ptr<Hero> hero);
    virtual void successfulEncount(std::shared_ptr<Hero> hero);
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
    std::function<void(Island (*), std::shared_ptr<Hero>)> _successfulEncount;
    bool _firstTimeStep = true;
public:
    bool isCleared;
    Island(World* world, QString roomName, QString roomDescr, int n, int e, int s, int w);
    Island(World* world, QString roomName, QString roomDescr,
           int n, int e, int s, int w, std::function<void(Island (*), std::shared_ptr<Hero>)> heroStepped,
           std::function<void(Island (*), std::shared_ptr<Hero>)> successfulEncount);
    virtual void heroStepped(std::shared_ptr<Hero> hero) override;
    virtual void successfulEncount(std::shared_ptr<Hero> hero) override;
    virtual bool isShopAvailable() override;
    bool firstTimeStep() const;
};

class E1Island : public Room {
private:
    bool _isMapCollected = false;
    bool _isOver = false;
public:
    E1Island(World* world, int n, int e, int s, int w);
    virtual void heroStepped(std::shared_ptr<Hero> hero) override;
    virtual void successfulEncount(std::shared_ptr<Hero> hero) override;
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
    void sendWorldChanged();
    void grantItem(std::shared_ptr<Hero> hero, std::shared_ptr<Item> item);
    void heroMoved(std::shared_ptr<Hero> hero, int index);
signals:
    void worldChanged();
    void encounter(EncounterType type, std::shared_ptr<Hero> enemy);
    void loudAddItem(std::shared_ptr<Item> item);
public slots:
    void encounterSuccessful(int index, std::shared_ptr<Hero> hero);
};

#endif // WORLD_H
