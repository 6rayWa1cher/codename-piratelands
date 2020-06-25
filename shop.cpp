#include "shop.h"
#include <iostream>
QMap<ItemType, QList<std::shared_ptr<Item> > > Shop::globalShop() const {
    return _globalShop;
}

Shop::Shop(QObject *parent) : QObject(parent) {
    for (int i = 0; i <= int(LAST_ITEM_TYPE); ++i) {
        _globalShop[ItemType(i)] = QList<std::shared_ptr<Item>>();
    }
}

bool Shop::shopToHeroTrade(std::shared_ptr<Hero> hero, std::shared_ptr<Item> item) {
    auto& itemGroupInShop = _globalShop[item->getType()];
    if (!itemGroupInShop.contains(item)) {
        throw new std::exception();
    }
    if (hero->changeMoney(-static_cast<int64_t>(item->price))) {
        try {
            hero->addItem(item);
        } catch(UniqueItemException e) {
            hero->changeMoney(static_cast<int64_t>(item->price));
            return false;
        }

        this->removeItem(item);
        return true;
    }
    return false;
}
bool Shop::heroToShopTrade(std::shared_ptr<Hero> hero, std::shared_ptr<Item> item) {
    auto& itemGroupInShop = hero->inventory()[item->getType()];
    if (!itemGroupInShop.contains(item)) {
        throw new std::exception();
    }
    if (hero->changeMoney(static_cast<int64_t>(item->price))) {
        auto replaceItem = item->changeAmount(-1);
        this->addItem(item);
        hero->removeItem(item);
        if(replaceItem) hero->addItem(replaceItem);
        return true;
    }
    return false;
}

void Shop::addItem(std::shared_ptr<Item> item) {
    auto& itemGroupInShop = _globalShop[item->getType()];
    int i = 0;
    for(i = 0; i < itemGroupInShop.size(); i++) {
        if(itemGroupInShop[i]->name == item->name &&
           itemGroupInShop[i]->description == item->description &&
           itemGroupInShop[i]->price == item->price) break;
    }
    if (i == itemGroupInShop.size()) {
        itemGroupInShop.push_back(item);
    }/* else {
        if(item->getType() == ItemType::SHIP_CONSUMABLE) return;
        auto newItem = item->changeAmount(1);
        removeItem(itemGroupInShop[i]);
        itemGroupInShop.append(newItem);
    }*/ // Не раскомментировать, убьёт
    emit shopChanged(_globalShop);
}

void Shop::removeItem(std::shared_ptr<Item> item) {
    auto& itemGroupInShop = _globalShop[item->getType()];
    if (!itemGroupInShop.contains(item)) {
        std::cerr << "Can't remove item which isn't presented" << std::endl;
        return;
    }
    if(item->getType() == ItemType::SHIP_CONSUMABLE) return;
    auto newItem = item->changeAmount(-1);
    if (newItem) {
        itemGroupInShop.replace(itemGroupInShop.indexOf(item), newItem);
    } else {
        itemGroupInShop.removeOne(item);
    }
    emit shopChanged(_globalShop);
}
