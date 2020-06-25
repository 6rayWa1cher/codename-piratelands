#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <memory>
#include "battleitemeffect.h"
#include "hero.h"

class Game;

enum class ItemType {
    KEY_ITEM = 0,
    SHIP_CONSUMABLE,
    SHIP_BOARDING_TEAM,
    SHIP_CANNONS,
    SHIP_HULL,
    SHIP_SAIL
};

const ItemType LAST_ITEM_TYPE = ItemType::SHIP_SAIL;

class UniqueItemException : public std::exception {

};

// ABSTRACT SECTION

class Item : public std::enable_shared_from_this<Item> {
public:
    const QString name;
    const QString description;
    const uint32_t price;
    uint32_t amount;
    Item(QString name, QString description, uint32_t price = 0, uint32_t amount = 1);
    virtual ItemType getType() const noexcept = 0;
    virtual std::shared_ptr<Item> changeAmount(int delta) const = 0;
};
bool operator==(const Item& i1, const Item& i2);
bool operator!=(const Item& i1, const Item& i2);

class KeyItem : public Item {
public:
    KeyItem(QString name, QString description, uint32_t price = 0);
    ItemType getType() const noexcept override;
    virtual void use(std::shared_ptr<Game> game) const = 0;
    std::shared_ptr<Item> changeAmount(int delta) const override;
};

class ShipItem : public Item {
public:
    ShipItem(QString name, QString description, uint32_t price = 0, uint32_t amount = 1);
};

class ShipConsumable;

struct ShipConsumableReturn {
    std::shared_ptr<ShipConsumable> consumeResult = nullptr;
    std::shared_ptr<BattleItemEffect> makeEffect = nullptr;
    bool effectOnOther = false;
    int8_t effectTurns = -1;
};

class ShipConsumable : public ShipItem {
public:
    ShipConsumable(QString name, QString description, uint32_t price = 0,
                   uint32_t amount = 1);
    virtual ShipConsumableReturn consume(Enemy* hero) = 0;
    ItemType getType() const noexcept override;
    std::shared_ptr<Item> changeAmount(int delta) const override;
    virtual std::shared_ptr<ShipConsumable> clone() const = 0;
};

class ShipEquipment : public ShipItem {
public:
    ShipEquipment(QString name, QString description, uint32_t price = 0);
    std::shared_ptr<Item> changeAmount(int delta) const override;
    virtual void equip(Hero* hero) = 0;
};

class ShipBoardingTeam : public ShipEquipment {
public:
    const uint8_t baseBoardingPower;
    ShipBoardingTeam(QString name, QString description, uint8_t baseBoardingPower,
                     uint32_t price = 0);
    void equip(Hero* hero) override;
    ItemType getType() const noexcept override;
};

class ShipCannons : public ShipEquipment {
public:
    const uint8_t baseAttack;
    const uint8_t baseAccuracy;
    const uint8_t weight;
    ShipCannons(QString name, QString description, uint8_t baseAttack,
                uint8_t baseAccuracy, uint8_t weight, uint32_t price = 0);
    void equip(Hero* hero) override;
    ItemType getType() const noexcept override;
};

class ShipHull : public ShipEquipment {
public:
    const uint8_t baseHealth;
    const uint8_t baseArmor;
    const uint8_t evasionDecreasement;
    const uint8_t escapeDecreasement;
    const uint8_t baseCarryingCapacity;
    const uint8_t boardingPowerDecreasement;
    ShipHull(QString name, QString description, uint32_t baseHealth, uint8_t baseArmor,
             uint8_t evasionDecreasement, uint8_t escapeDecreasement,
             uint8_t baseCarryingCapacity, uint8_t boardingPowerDecreasement,
             uint32_t price = 0);
    void equip(Hero* hero) override;
    ItemType getType() const noexcept override;
};

class ShipSail : public ShipEquipment {
public:
    const uint8_t baseEvasion;
    const uint8_t baseEscape;
    ShipSail(QString name, QString description, uint8_t baseEvasion, uint8_t baseEscape,
             uint32_t price = 0);
    void equip(Hero* hero) override;
    ItemType getType() const noexcept override;
};

// IMPLEMENTATIONS SECTION

class MapPiece : public KeyItem {
public:
    MapPiece(int partNumber);
    void use(std::shared_ptr<Game> game) const override;
};

class Map : public KeyItem {
public:
    Map(QString name, QString description, uint32_t price = 0);
    void use(std::shared_ptr<Game> game) const override;
};

class HealingItem : public ShipConsumable {
public:
    const uint16_t heal;
    HealingItem(QString name, QString description, uint16_t heal, uint32_t price = 0,
                uint32_t amount = 1);
    ShipConsumableReturn consume(Enemy *enemy) override;
    std::shared_ptr<ShipConsumable> clone() const override;
};

class BombItem : public ShipConsumable {
public:
    const uint32_t damage;
    BombItem(QString name = "Bomb", QString description = "With a 50% chance, deals damage per turn to the enemy for two turns", uint32_t damage = 40, uint32_t price = 500,
         uint32_t amount = 1);
    ShipConsumableReturn consume(Enemy *enemy) override;
    std::shared_ptr<ShipConsumable> clone() const override;
};

class BuckshotItem : public ShipConsumable {
public:
    BuckshotItem(QString name = "Buckshot", QString description = "With a 50% chance reduces the boarding power of the enemy team by 50%", uint32_t price = 500,
                 uint32_t amount = 1);
    ShipConsumableReturn consume(Enemy *enemy) override;
    std::shared_ptr<ShipConsumable> clone() const override;
};

const std::vector<std::shared_ptr<Item>> items = {
    std::make_shared<MapPiece>(1),         // 0
    std::make_shared<MapPiece>(2),         // 1
    std::make_shared<Map>("Карта архипелага", "Открывает карту архипелага", 500),   // 2
    std::make_shared<HealingItem>("Доски", "Используются для починки корпуса", 55, 150, 1),         // 3
    std::make_shared<HealingItem>("Стальные листы", "Используются для починки корпуса", 120, 250, 1),       // 4
    std::make_shared<BombItem>("Бомбы", "С шансом 50 % поджигают вражеский корабль. Противник получает урон в течении двух ходов", 40, 500, 1),           // 5
    std::make_shared<BuckshotItem>("Картечь", "С шансом 50 % поражает личный состав вражеского корабля. Сила снаряжения противника уменьшается", 500, 1),   // 6
    std::make_shared<ShipCannons>("Бомбарды", "Лёгкие пушки с минимальной мощностью поражения", 20, 30, 1, 250),      // 7
    std::make_shared<ShipCannons>("12-фунтовые пушки", "Обычные пушки, подойдут для любой задачи", 60, 50, 2, 400),         // 8
    std::make_shared<ShipCannons>("24-фунтовые пушки", "Тяжёлые пушки с высокой огневой мощью", 80, 75, 3, 850),         // 9
    std::make_shared<ShipBoardingTeam>("Корабельные крысы", "Бухие оборванцы", 20, 50),            // 10
    std::make_shared<ShipBoardingTeam>("Бывалые каперы", "Опытные моряки", 40, 150),          // 11
    std::make_shared<ShipBoardingTeam>("Морские волки", "Бывалые бойцы", 80, 750),           // 12
    std::make_shared<ShipHull>("Деревянный корпус", "Быстрый, но не грузоподъёмный", 120, 25, 0, 0, 1, 0, 2500),     // 13
    std::make_shared<ShipHull>("Полу-бронированный корпус", "Корпус-универсал", 150, 35, 53, 28, 2, 50, 1000),   // 14
    std::make_shared<ShipHull>("Бронированный корпус", "Невероятно крепкий корпус", 170, 75, 60, 57, 3, 75, 2500),       // 15
    std::make_shared<ShipSail>("2 мачты, обычные паруса", "Стандартные паруса", 41, 35, 50),          // 16
    std::make_shared<ShipSail>("2 мачты, отличные паруса", "Укреплённые паруса", 55, 50, 200),        // 17
    std::make_shared<ShipSail>("3 мачты, обычные паруса", "Стандартные паруса", 75, 70, 1000)   // 18
};


#endif // ITEM_H
