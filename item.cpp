#include "item.h"
#include "game.h"

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

void ShipCannons::equip(Hero* hero) {
    auto cannons = hero->cannons();
    if(cannons) hero->addItem(cannons);
    hero->removeItem(std::make_shared<ShipCannons>(*this));
    hero->equipCannons(std::make_shared<ShipCannons>(*this));
}

ItemType ShipCannons::getType() const noexcept {
    return ItemType::SHIP_CANNONS;
}


ShipHull::ShipHull(QString name, QString description, uint32_t additionalHealth,
                   uint8_t escapeDecreasement, uint8_t carryingCapacity, uint32_t price) :
    ShipEquipment(name, description, price), additionalHealth(additionalHealth),
    escapeDecreasement(escapeDecreasement), carryingCapacity(carryingCapacity) {

}

void ShipHull::equip(Hero* hero) {
    auto hull = hero->hull();
    if(hull) hero->addItem(hull);
    hero->removeItem(std::make_shared<ShipHull>(*this));
    hero->equipHull(std::make_shared<ShipHull>(*this));

}

ItemType ShipHull::getType() const noexcept {
    return ItemType::SHIP_HULL;
}

ShipSail::ShipSail(QString name, QString description, uint8_t escapeIncreasement,
                   uint32_t price) : ShipEquipment(name, description, price),
    escapeIncreasement(escapeIncreasement) {

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





