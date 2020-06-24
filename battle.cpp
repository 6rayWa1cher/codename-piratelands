#include "battle.h"

#include "game.h"
#include <iostream>

Battle::Battle(QObject *parent, Game* game) :
    QObject(parent), _currentEnemy(nullptr), _game(game), _rd(), _gen(_rd())
{
    _gen.seed(time(0));
}

int Battle::stepNumber() const {
    return _stepNumber;
}

bool Battle::isBattle() const noexcept {
    return _stepNumber != -1;
}

void Battle::startBattle(std::shared_ptr<Enemy> enemy) {
    _currentEnemy = enemy;
    _stepNumber = 1;
}

std::shared_ptr<Enemy> Battle::currentEnemy() const {
    return _currentEnemy;
}

uint8_t Battle::genAndNormalize(uint8_t mostCommonValue, uint8_t max) {
    std::normal_distribution<> d{double(mostCommonValue), 15};
    double p = d(_gen);
    if (p < 0) p = 0;
    if (p > max) p = max;
    return p;
}

int Battle::tryAttack(std::shared_ptr<Enemy> attacker, std::shared_ptr<Enemy> defender) {
    emit battleEvent(attacker->name() + " начинает атаку");
    uint8_t attackerAccuracy = attacker->cannons()->baseAccuracy;
    uint8_t p1 = genAndNormalize(0.75 * attackerAccuracy, attackerAccuracy);
    uint8_t defenderEvasion = defender->sail()->baseEvasion *
            (100 - defender->hull()->evasionDecreasement) / 100;
    uint8_t p2 = genAndNormalize(0.75 * defenderEvasion, defenderEvasion);
    if (p1 < p2) {
        emit battleEvent(attacker->name() + " промахнулся! " + std::to_string(p1).c_str() + "<" + std::to_string(p2).c_str());
        return 0;
    }
    uint8_t atk = attacker->cannons()->baseAttack;
    uint8_t arm = defender->hull()->baseArmor;
    uint32_t damage = atk * (100 - arm) / 100;
    emit battleEvent(attacker->name() + " попал! " + std::to_string(p1).c_str() + ">=" + std::to_string(p2).c_str() + ", нанесено " + std::to_string(damage).c_str() + " урона");
    if (defender->health() < damage) {
        emit battleEvent(defender->name() + " потоплен!");
        return 2;
    }
    defender->setHealth(defender->health() - damage);
    return 1;
}

int Battle::tryBoarding(std::shared_ptr<Enemy> attacker, std::shared_ptr<Enemy> defender) {
    emit battleEvent(attacker->name() + " делает попытку взять на абордаж!");
    uint8_t attackerEscape = attacker->sail()->baseEscape * (100 - attacker->hull()->escapeDecreasement) / 100;
    uint8_t p1 = genAndNormalize(0.75 * attackerEscape, attackerEscape);
    uint8_t defenderEscape = defender->sail()->baseEscape * (100 - defender->hull()->escapeDecreasement) / 100;
    uint8_t p2 = genAndNormalize(0.75 * defenderEscape, defenderEscape);
    if (p1 < p2) {
        emit battleEvent(defender->name() + " уклонился от абордажа. " + std::to_string(p1).c_str() + "<" + std::to_string(p2).c_str());
        return 0;
    }
    emit battleEvent("Абордаж начался!");
    uint8_t attackerBoardingPower = attacker->team()->baseBoardingPower * (100 - attacker->hull()->boardingPowerDecreasement) / 100;
    uint8_t p3 = genAndNormalize(0.75 * attackerBoardingPower, attackerBoardingPower);
    uint8_t defenderBoardingPower = defender->team()->baseBoardingPower * (100 - defender->hull()->boardingPowerDecreasement) / 100;
    uint8_t p4 = genAndNormalize(0.75 * defenderBoardingPower, defenderBoardingPower);
    if (p3 < p4) {
        emit battleEvent(QString("Абордаж не увенчался успехом! ") + std::to_string(p3).c_str() + "<" + std::to_string(p4).c_str());
        return 1;
    }
    emit battleEvent("Абордаж удался!");
    return 2;
}

int Battle::tryFlee(std::shared_ptr<Enemy> runner, std::shared_ptr<Enemy> chaser) {
    emit battleEvent(runner->name() + " пытается сбежать!");
    uint8_t attackerEscape = runner->sail()->baseEscape * (100 - runner->hull()->escapeDecreasement) / 100;
    uint8_t p1 = genAndNormalize(0.75 * attackerEscape, attackerEscape);
    uint8_t defenderEscape = chaser->sail()->baseEscape * (100 - chaser->hull()->escapeDecreasement) / 100;
    uint8_t p2 = genAndNormalize(0.75 * defenderEscape, defenderEscape);
    if (p1 < p2) {
        emit battleEvent(runner->name() + " не убежал. " + std::to_string(p1).c_str() + "<" + std::to_string(p2).c_str());
        return 0;
    }
    BattleWonResult result;
    result.flee = true;
    emit battleEvent(runner->name() + " убежал!");
    return 1;
}

void Battle::heroWon() {
    BattleWonResult bwr;
    _game->_hero->changeMoney(_currentEnemy->money());
    bwr.gold = _currentEnemy->money();
    uint8_t p1 = genAndNormalize(70, 100);
    std::cout << std::to_string(p1) << std::endl;
    if (p1 >= 75) {
        uint8_t t = std::uniform_int_distribution<>((int) ItemType::SHIP_BOARDING_TEAM, (int) ItemType::SHIP_SAIL)(_gen);
        std::shared_ptr<Item> item;
        bool equals = false;
        switch (t) {
        case (int) ItemType::SHIP_BOARDING_TEAM:
            item = _currentEnemy->team();
            equals = *_game->_hero->team() == *item;
            break;
        case (int) ItemType::SHIP_CANNONS:
            item = _currentEnemy->cannons();
            equals = *_game->_hero->cannons() == *item;
            break;
        case (int) ItemType::SHIP_HULL:
            item = _currentEnemy->hull();
            equals = *_game->_hero->hull() == *item;
            break;
        case (int) ItemType::SHIP_SAIL:
            item = _currentEnemy->sail();
            equals = *_game->_hero->sail() == *item;
            break;
        default:
            item = nullptr;
        }
        bwr.item = item;
        if (!equals) {
            try {
                _game->_hero->addItem(item);
                bwr.selled = false;
            } catch (UniqueItemException e) {
                equals = true;
            }
        }
        if (equals) {
            _game->_hero->changeMoney(item->price);
            bwr.selled = true;
        }
    }
    emit battleOver(_currentEnemy, bwr);
    (*_game->_world)[_game->_hero->currentRoom()]->successfulEncount(_game->_hero);
    _currentEnemy = nullptr;
}

void Battle::heroLost() {
    _game->_hero->resurrect();
    emit battleOver(_currentEnemy, BattleWonResult());
    _currentEnemy = nullptr;
}

void Battle::enemyTurn() {
    if (true) {
        int result = tryAttack(_currentEnemy, _game->_hero);
        switch (result) {
        case 0:
        case 1:
            emit turnOver();
            break;
        case 2:
            heroLost();
            break;
        }
    }
}

void Battle::attack() {
   int result = tryAttack(_game->_hero, _currentEnemy);
   switch (result) {
   case 0:
   case 1:
       enemyTurn();
       break;
   case 2:
       heroWon();
       break;
   }
}

void Battle::boarding() {
    int result = tryBoarding(_game->_hero, _currentEnemy);
    switch (result) {
    case 0:
    case 1:
        enemyTurn();
        break;
    case 2:
        heroWon();
        break;
    }
}

void Battle::clickItem(std::shared_ptr<Item> item) {
    emit battleEvent(_game->_hero->name() + " применил предмет " + item->name);
    std::dynamic_pointer_cast<ShipConsumable>(item)->consume(&*(_game->_hero));
    enemyTurn();
}

void Battle::fleeing() {
    int result = tryFlee(_game->_hero, _currentEnemy);
    switch (result) {
    case 0:
        enemyTurn();
        break;
    case 1:
        BattleWonResult result;
        result.flee = true;
        emit battleOver(_currentEnemy, result);
        _currentEnemy = nullptr;
        break;
    }
}

