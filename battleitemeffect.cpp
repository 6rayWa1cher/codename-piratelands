#include "battleitemeffect.h"


QString BattleItemEffect::name() const
{
    return _name;
}

QString BattleItemEffect::description() const
{
    return _description;
}

BattleItemEffect::BattleItemEffect(QString name, QString description) :
    _name(name), _description(description)
{

}

BombItemEffect::BombItemEffect() :
    AfterMoveEffect("Бомбы!", "Бомбы взрываются на вашем корабле!")
{

}

QString BombItemEffect::apply(std::shared_ptr<Enemy> enemy)
{
    if (enemy->health() < 40) {
        enemy->setHealth(0);
        return "Корабль взорвался!";
    } else {
        enemy->setHealth(enemy->health() - 40);
        return QString(("Снято 40 dmg: " + std::to_string(enemy->health() + 40) + " -> " + std::to_string(enemy->health())).c_str());
    }
}

StatsEffect::StatsEffect(QString name, QString description) :
    BattleItemEffect(name, description)
{

}

EffectType StatsEffect::getType() const
{
    return EffectType::STATS;
}

AfterMoveEffect::AfterMoveEffect(QString name, QString description) :
    BattleItemEffect(name, description)
{

}

EffectType AfterMoveEffect::getType() const
{
    return EffectType::AFTER_MOVE_ACTION;
}

BuckshotEffect::BuckshotEffect() : StatsEffect("Картечь", "Корабль был обстрелян картечью, сила команды ниже!")
{

}

uint8_t BuckshotEffect::apply(std::shared_ptr<Enemy> /*enemy*/, uint8_t raw, StatsType type)
{
    if (type == StatsType::BP) {
        return raw / 2;
    }
    return raw;
}
