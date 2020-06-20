#include "item.h"

Item::Item(QString name, QString description, uint32_t price, uint32_t amount) :
    name(name), description(description), price(price), amount(amount){

}

KeyItem::KeyItem(QString name, QString description, uint32_t price) :
    Item(name, description, price, 1){

}


ItemType KeyItem::getType() const noexcept {
    return ItemType::KEY_ITEM;
}

std::shared_ptr<Item> KeyItem::changeAmount(int delta) const
{
    switch(delta) {
    case -1:
        return nullptr;
    default:
        throw new std::exception();
    }
}

HeroItem::HeroItem(QString name, QString description, uint32_t price,
                   uint32_t amount) :
    Item(name, description, price, amount){

}

HeroConsumable::HeroConsumable(QString name, QString description, uint32_t price,
                               uint32_t amount) :
    HeroItem(name, description, price, amount) {

}

ItemType HeroConsumable::getType() const noexcept {
    return ItemType::HERO_CONSUMABLE;
}

std::shared_ptr<Item> HeroConsumable::changeAmount(int delta) const
{
    if (delta < 0 && static_cast<uint32_t>(-delta) > amount) {
        return nullptr;
    }
    std::shared_ptr<HeroConsumable> copy = this->clone();
    copy->amount += delta;
    return copy;
}

HeroEquipment::HeroEquipment(QString name, QString description, uint32_t price) :
    HeroItem(name, description, price, 1) {

}


std::shared_ptr<Item> HeroEquipment::changeAmount(int delta) const
{
    switch(delta) {
    case -1:
        return nullptr;
    default:
        throw new std::exception();
    }
}

HeroWeapon::HeroWeapon(QString name, QString description, uint32_t rawDamage,
                       uint8_t probability, uint32_t price) :
    HeroEquipment(name, description, price), rawDamage(rawDamage), probability(probability) {

}

HeroMeleeWeapon::HeroMeleeWeapon(QString name, QString description, uint32_t rawDamage,
                                 uint8_t probability, uint32_t price) :
    HeroWeapon(name, description, rawDamage, probability, price) {

}

void HeroMeleeWeapon::equip(Hero* hero) {
    //TODO: EQUIP
}

ItemType HeroMeleeWeapon::getType() const noexcept {
    return ItemType::HERO_MELEE_WEAPON;
}


HeroRangeWeapon::HeroRangeWeapon(QString name, QString description, uint32_t rawDamage,
                                 uint8_t probability, uint32_t price) :
    HeroWeapon(name, description, rawDamage, probability, price) {

}

void HeroRangeWeapon::equip(Hero* hero) {
    //TODO: EQUIP
}

ItemType HeroRangeWeapon::getType() const noexcept {
    return ItemType::HERO_RANGE_WEAPON;
}


HeroArmor::HeroArmor(QString name, QString description, uint8_t hitDecreasement,
                     uint8_t escapeDecreasement, uint32_t price) :
    HeroEquipment(name, description, price), hitDecreasement(hitDecreasement),
    escapeDecreasement(escapeDecreasement) {

}

void HeroArmor::equip(Hero* hero) {
    //TODO: EQUIP
}

ItemType HeroArmor::getType() const noexcept {
    return ItemType::HERO_ARMOR;
}

ShipItem::ShipItem(QString name, QString description, uint32_t price,
                   uint32_t amount) :
    Item(name, description, price, amount){

}

ShipConsumable::ShipConsumable(QString name, QString description, uint32_t price,
                               uint32_t amount) :
    ShipItem(name, description, price, amount) {

}

ItemType ShipConsumable::getType() const noexcept {
    return ItemType::SHIP_CONSUMABLE;
}

std::shared_ptr<Item> ShipConsumable::changeAmount(int delta) const
{
    if (delta < 0 && static_cast<uint32_t>(-delta) > amount) {
        return nullptr;
    }
    std::shared_ptr<ShipConsumable> copy = this->clone();
    copy->amount += delta;
    return copy;
}


ShipEquipment::ShipEquipment(QString name, QString description, uint32_t price) :
    ShipItem(name, description, price, 1){

}

std::shared_ptr<Item> ShipEquipment::changeAmount(int delta) const
{
    switch(delta) {
    case -1:
        return nullptr;
    default:
        throw new std::exception();
    }
}

ShipCannons::ShipCannons(QString name, QString description, uint8_t rawDamage,
                         uint8_t probability, uint8_t weight, uint32_t price) :
    ShipEquipment(name, description, price), rawDamage(rawDamage),
    probability(probability), weight(weight) {

}

void ShipCannons::equip(Ship* ship) {
    //TODO: EQUIP
}

ItemType ShipCannons::getType() const noexcept {
    return ItemType::SHIP_CANNONS;
}


ShipHull::ShipHull(QString name, QString description, uint32_t additionalHealth,
                   uint8_t escapeDecreasement, uint8_t carryingCapacity, uint32_t price) :
    ShipEquipment(name, description, price), additionalHealth(additionalHealth),
    escapeDecreasement(escapeDecreasement), carryingCapacity(carryingCapacity) {

}

void ShipHull::equip(Ship* ship) {
    //TODO: EQUIP
}

ItemType ShipHull::getType() const noexcept {
    return ItemType::SHIP_HULL;
}

ShipSail::ShipSail(QString name, QString description, uint8_t escapeIncreasement,
                   uint32_t price) : ShipEquipment(name, description, price),
    escapeIncreasement(escapeIncreasement) {

}

void ShipSail::equip(Ship* ship) {
    //TODO: EQUIP
}

ItemType ShipSail::getType() const noexcept {
    return ItemType::SHIP_SAIL;
}







