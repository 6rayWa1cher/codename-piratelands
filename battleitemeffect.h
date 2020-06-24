#ifndef BATTLEITEMEFFECT_H
#define BATTLEITEMEFFECT_H

#include <memory>
#include "hero.h"

enum StatsType {
    BP
};

const StatsType LAST_STATS_TYPE = BP;

enum EffectType {
    STATS = 0,
    AFTER_MOVE_ACTION
};

const EffectType LAST_EFFECT_TYPE = AFTER_MOVE_ACTION;

class BattleItemEffect
{
private:
    QString _name;
    QString _description;
public:
    BattleItemEffect(QString name, QString description);
    QString name() const;
    QString description() const;
    virtual EffectType getType() const = 0;
};

class AfterMoveEffect : public BattleItemEffect {
public:
    AfterMoveEffect(QString name, QString description);
    virtual QString apply(std::shared_ptr<Enemy> enemy) = 0;
    EffectType getType() const override;
};

class StatsEffect : public BattleItemEffect {
public:
    StatsEffect(QString name, QString description);
    virtual uint8_t apply(std::shared_ptr<Enemy> enemy, uint8_t raw, StatsType type) = 0;
    EffectType getType() const override;
};

class BombItemEffect : public AfterMoveEffect {
public:
    BombItemEffect();
    QString apply(std::shared_ptr<Enemy> enemy) override;
};

class BuckshotEffect : public StatsEffect {
public:
    BuckshotEffect();
    uint8_t apply(std::shared_ptr<Enemy> enemy, uint8_t raw, StatsType type) override;
};

#endif // BATTLEITEMEFFECT_H
