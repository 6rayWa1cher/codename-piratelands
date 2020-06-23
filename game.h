#ifndef GAME_H
#define GAME_H
#include <QObject>
#include "battle.h"
#include "hero.h"
#include "shop.h"

class Game : public QObject {
    Q_OBJECT
public:
    std::shared_ptr<Hero> _hero;
    std::shared_ptr<World> _world;
    std::shared_ptr<Shop> _shop;
    std::shared_ptr<Battle> _battle;
    Game(QString name);
public slots:
    void heroMoved(int index);
};

#endif // GAME_H
