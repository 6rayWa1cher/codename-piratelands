#ifndef BATTLE_H
#define BATTLE_H

#include "battleitemeffect.h"
#include "hero.h"

#include <QObject>
#include <memory>
#include <random>

struct BattleWonResult {
    bool flee = false;
    uint32_t gold = 0;
    std::shared_ptr<Item> item = nullptr;
    bool selled = false;
};

class Game;

class Battle : public QObject
{
    Q_OBJECT
private:
    std::shared_ptr<Enemy> _currentEnemy;
    Game* _game;
    std::random_device _rd;
    std::mt19937 _gen;
    int _stepNumber = -1;
    std::shared_ptr<BattleItemEffect> _heroEffect = nullptr;
    int8_t _heroEffectTurnsLeft = 0;
    std::shared_ptr<BattleItemEffect> _enemyEffect = nullptr;
    int8_t _enemyEffectTurnsLeft = 0;
    uint8_t genAndNormalize(uint8_t mostCommonValue, uint8_t max = 100);
    uint8_t appendEffectOn(StatsType type, uint8_t raw, std::shared_ptr<Enemy> entity);
    int tryAttack(std::shared_ptr<Enemy> attacker, std::shared_ptr<Enemy> defender);
    int tryBoarding(std::shared_ptr<Enemy> attacker, std::shared_ptr<Enemy> defender);
    int tryFlee(std::shared_ptr<Enemy> runner, std::shared_ptr<Enemy> chaser);
    void heroWon();
    void heroLost();
    void enemyTurn();
public:
    Battle(QObject *parent, Game* game);

    int stepNumber() const;

    bool isBattle() const noexcept;

    std::shared_ptr<Enemy> currentEnemy() const;

    const QList<QString>& logs() const;

    std::shared_ptr<BattleItemEffect> heroEffect() const;

    int8_t heroEffectTurnsLeft() const;

    std::shared_ptr<BattleItemEffect> enemyEffect() const;

    int8_t enemyEffectTurnsLeft() const;

signals:
    void turnOver();
    void battleEvent(QString description);
    void battleOver(std::shared_ptr<Enemy> enemy, BattleWonResult result);
public slots:
    void startBattle(std::shared_ptr<Enemy> enemy);
    void attack();
    void boarding();
    void clickItem(std::shared_ptr<Item> item);
    void fleeing();
};

#endif // BATTLE_H
