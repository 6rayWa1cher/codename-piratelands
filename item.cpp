#include "item.h"
#include "game.h"
#include <algorithm>
#include <iostream>

Item::Item(QString name, QString description, uint32_t price, uint32_t amount) :
    name(name), description(description), price(price), amount(amount){

}

KeyItem::KeyItem(QString name, QString description, uint32_t price) :
    Item(name, description, price, 1){

}


ItemType KeyItem::getType() const noexcept {
    return ItemType::KEY_ITEM;
}

std::shared_ptr<Item> KeyItem::changeAmount(int delta) const {
    switch(delta) {
    case -1:
        return nullptr;
    default:
        throw UniqueItemException();
    }
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

std::shared_ptr<Item> ShipConsumable::changeAmount(int delta) const {
    if (delta < 0 && static_cast<uint32_t>(-delta) >= amount) {
        return nullptr;
    }
    std::shared_ptr<ShipConsumable> copy = this->clone();
    copy->amount += delta;
    return copy;
}


ShipEquipment::ShipEquipment(QString name, QString description, uint32_t price) :
    ShipItem(name, description, price, 1){

}

std::shared_ptr<Item> ShipEquipment::changeAmount(int delta) const {
    switch(delta) {
    case -1:
        return nullptr;
    default:
        throw UniqueItemException();
    }
}

ShipCannons::ShipCannons(QString name, QString description, uint8_t baseAttack,
                         uint8_t baseAccuracy, uint8_t weight, uint32_t price) :
    ShipEquipment(name, description, price), baseAttack(baseAttack),
    baseAccuracy(baseAccuracy), weight(weight) {

}

void ShipCannons::equip(Hero* hero) {
    if(weight > hero->hull()->baseCarryingCapacity) return;   //TODO implement notification window
    auto cannons = hero->cannons();
    if(cannons) hero->addItem(cannons);
    hero->removeItem(std::make_shared<ShipCannons>(*this));
    hero->equipCannons(std::make_shared<ShipCannons>(*this));
}

ItemType ShipCannons::getType() const noexcept {
    return ItemType::SHIP_CANNONS;
}


ShipHull::ShipHull(QString name, QString description, uint32_t baseHealth, uint8_t baseArmor,
                   uint8_t evasionDecreasement, uint8_t escapeDecreasement,
                   uint8_t baseCarryingCapacity, uint8_t boardingPowerDecreasement,
                   uint32_t price) :
    ShipEquipment(name, description, price), baseHealth(baseHealth),
    baseArmor(baseArmor), evasionDecreasement(evasionDecreasement),
    escapeDecreasement(escapeDecreasement), baseCarryingCapacity(baseCarryingCapacity),
    boardingPowerDecreasement(boardingPowerDecreasement){

}

void ShipHull::equip(Hero* hero) {
    if(baseCarryingCapacity < hero->cannons()->weight) return; // TODO implement notification window
    auto hull = hero->hull();
    if(hull) hero->addItem(hull);
    hero->removeItem(std::make_shared<ShipHull>(*this));
    hero->equipHull(std::make_shared<ShipHull>(*this));

}

ItemType ShipHull::getType() const noexcept {
    return ItemType::SHIP_HULL;
}

ShipSail::ShipSail(QString name, QString description, uint8_t baseEvasion, uint8_t baseEscape,
                   uint32_t price) : ShipEquipment(name, description, price),
    baseEvasion(baseEvasion), baseEscape(baseEscape) {

}

void ShipSail::equip(Hero* hero) {
    auto sail = hero->sail();
    if(sail) hero->addItem(sail);
    hero->removeItem(std::make_shared<ShipSail>(*this));
    hero->equipSail(std::make_shared<ShipSail>(*this));
}

ItemType ShipSail::getType() const noexcept {
    return ItemType::SHIP_SAIL;
}

MapPiece::MapPiece(int partNumber) :
    KeyItem(("Карта часть#" + std::to_string(partNumber)).c_str(),
            (partNumber == 1 ? "От Ямайки на восток до суши..." : "затем на юг."),
            0) {

}

void MapPiece::use(std::shared_ptr<Game> /*game*/) const
{

}






ShipBoardingTeam::ShipBoardingTeam(QString name, QString description, uint8_t baseBoardingPower,
                                   uint32_t price) :
    ShipEquipment(name, description, price), baseBoardingPower(baseBoardingPower)
{

}

void ShipBoardingTeam::equip(Hero *hero) {
    auto team = hero->team();
    if(team) hero->addItem(team);
    hero->removeItem(std::make_shared<ShipBoardingTeam>(*this));
    hero->equipTeam(std::make_shared<ShipBoardingTeam>(*this));
}

ItemType ShipBoardingTeam::getType() const noexcept {
    return ItemType::SHIP_BOARDING_TEAM;
}

HealingItem::HealingItem(QString name, QString description, uint16_t heal, uint32_t price, uint32_t amount)
    : ShipConsumable(name, description, price, amount), heal(heal){

}

std::shared_ptr<ShipConsumable> HealingItem::consume(Hero *hero) {
    uint16_t maxUnitHealth = hero->maxHealth();
    uint16_t unitHealth = hero->health();
    if(maxUnitHealth == unitHealth) return this->clone();

    uint16_t newHealth = unitHealth + heal;
    auto newItem = this->changeAmount(-1);
    newHealth = std::min(maxUnitHealth, newHealth);

    hero->setHealth(newHealth);
    hero->removeItem(this->clone());
    if(newItem) hero->addItem(newItem);
    return std::dynamic_pointer_cast<ShipConsumable>(newItem);
}

std::shared_ptr<ShipConsumable> HealingItem::clone() const {
    return std::static_pointer_cast<ShipConsumable>(
                std::make_shared<HealingItem>(this->name, this->description, this->heal, this->price, this->amount)
                );
}

BombItem::BombItem(QString name, QString description, uint32_t damage, uint32_t price, uint32_t amount)
    : ShipConsumable(name, description, price, amount), damage(damage) {

}

std::shared_ptr<ShipConsumable> BombItem::consume(Hero *hero) {
    // TODO add implementation
    return nullptr;
}

std::shared_ptr<ShipConsumable> BombItem::clone() const {
    return std::static_pointer_cast<ShipConsumable>(
                std::make_shared<BombItem>(this->name, this->description, this->damage, this->price, this->amount)
                );
}

BuckshotItem::BuckshotItem(QString name, QString description, uint32_t price, uint32_t amount)
    : ShipConsumable(name, description, price, amount) {

}

std::shared_ptr<ShipConsumable> BuckshotItem::consume(Hero *hero) {
    // TODO using
}

std::shared_ptr<ShipConsumable> BuckshotItem::clone() const {
    return std::static_pointer_cast<ShipConsumable>(
                std::make_shared<BuckshotItem>(this->name, this->description, this->price, this->amount)
                );
}

bool operator==(const Item &i1, const Item &i2) {
    return i1.name == i2.name &&
            i1.description == i2.description;
}

bool operator!=(const Item &i1, const Item &i2) {
    return !(i1 == i2);
}
