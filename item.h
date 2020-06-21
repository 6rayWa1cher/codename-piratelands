#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <memory>
#include "hero.h"
class Game;

enum class ItemType {
    KEY_ITEM = 0,
    HERO_CONSUMABLE,
    HERO_MELEE_WEAPON,
    HERO_RANGE_WEAPON,
    HERO_ARMOR,
    SHIP_CONSUMABLE,
    SHIP_CANNONS,
    SHIP_HULL,
    SHIP_SAIL
};

const ItemType LAST_ITEM_TYPE = ItemType::SHIP_SAIL;

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
    virtual void equip(Hero* hero) = 0;
};

class KeyItem : public Item {
public:
    KeyItem(QString name, QString description, uint32_t price = 0);
    ItemType getType() const noexcept override;
    virtual void use(std::shared_ptr<Game> game) const = 0;
    std::shared_ptr<Item> changeAmount(int delta) const override;
};

class HeroItem : public Item {
public:
    HeroItem(QString name, QString description, uint32_t price = 0, uint32_t amount = 1);
};

class HeroConsumable : public HeroItem {
public:
    HeroConsumable(QString name, QString description, uint32_t price = 0,
                   uint32_t amount = 1);
    virtual std::shared_ptr<HeroConsumable> consume(Hero* hero) = 0;
    ItemType getType() const noexcept override;
    std::shared_ptr<Item> changeAmount(int delta) const override;
    virtual std::shared_ptr<HeroConsumable> clone() const = 0;
};

class HeroEquipment : public HeroItem {
public:
    HeroEquipment(QString name, QString description, uint32_t price = 0);
    //virtual void equip(Hero* hero) = 0;
    std::shared_ptr<Item> changeAmount(int delta) const override;
};

class HeroWeapon : public HeroEquipment {
public:
    const uint32_t rawDamage;
    const uint8_t probability;
    HeroWeapon(QString name, QString description, uint32_t rawDamage,
               uint8_t probability, uint32_t price = 0);
};

class HeroMeleeWeapon : public HeroWeapon {
public:
    HeroMeleeWeapon(QString name, QString description, uint32_t rawDamage,
                    uint8_t probability, uint32_t price = 0);
    void equip(Hero *hero) override;
    ItemType getType() const noexcept override;
};

class HeroRangeWeapon : public HeroWeapon {
public:
    HeroRangeWeapon(QString name, QString description, uint32_t rawDamage,
                    uint8_t probability, uint32_t price = 0);
    void equip(Hero *hero) override;
    bool reload(Hero* hero);
    ItemType getType() const noexcept override;
};

class HeroArmor : public HeroEquipment {
public:
    const uint8_t hitDecreasement;
    const uint8_t escapeDecreasement;
    HeroArmor(QString name, QString description, uint8_t hitDecreasement,
              uint8_t escapeDecreasement, uint32_t price = 0);
    void equip(Hero *hero) override;
    ItemType getType() const noexcept override;
};

class ShipItem : public Item {
public:
    ShipItem(QString name, QString description, uint32_t price = 0, uint32_t amount = 1);
    virtual void equip(Hero* hero) = 0;
};

class ShipConsumable : public ShipItem {
public:
    ShipConsumable(QString name, QString description, uint32_t price = 0,
                   uint32_t amount = 1);
    virtual std::shared_ptr<HeroConsumable> consume(Hero* hero) = 0;
    ItemType getType() const noexcept override;
    std::shared_ptr<Item> changeAmount(int delta) const override;
    virtual std::shared_ptr<ShipConsumable> clone() const = 0;
};

class ShipEquipment : public ShipItem {
public:
    ShipEquipment(QString name, QString description, uint32_t price = 0);
    std::shared_ptr<Item> changeAmount(int delta) const override;
};

class ShipCannons : public ShipEquipment {
public:
    const uint8_t rawDamage;
    const uint8_t probability;
    const uint8_t weight;
    ShipCannons(QString name, QString description, uint8_t rawDamage,
                uint8_t probability, uint8_t weight, uint32_t price = 0);
    void equip(Hero* hero) override;
    ItemType getType() const noexcept override;
};

class ShipHull : public ShipEquipment {
public:
    const uint32_t additionalHealth;
    const uint8_t escapeDecreasement;
    const uint8_t carryingCapacity;
    ShipHull(QString name, QString description, uint32_t additionalHealth,
             uint8_t escapeDecreasement, uint8_t carryingCapacity, uint32_t price = 0);
    void equip(Hero* hero) override;
    ItemType getType() const noexcept override;
};

class ShipSail : public ShipEquipment {
public:
    const uint8_t escapeIncreasement;
    ShipSail(QString name, QString description, uint8_t escapeIncreasement,
             uint32_t price = 0);
    void equip(Hero* hero) override;
    ItemType getType() const noexcept override;
};

// IMPLEMENTATIONS SECTION

#endif // ITEM_H
