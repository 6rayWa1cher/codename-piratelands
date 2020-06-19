#ifndef HERO_H
#define HERO_H
#include <QObject>
#include <memory>
#include "world.h"

class Hero : public QObject {
    Q_OBJECT

protected:
    QString _name;
    int _health;
    int _money;
    std::shared_ptr<World> _world;
    int _currentRoom;

public:
    Hero(QString name, std::shared_ptr<World> world);
    void move(Direction dir);
signals:
    void hero_moved(int room);
};

#endif // HERO_H