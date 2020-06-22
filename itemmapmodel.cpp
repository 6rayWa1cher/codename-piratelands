#include "itemmapmodel.h"
#include "item.h"
#include <memory.h>
#include "game.h"


ItemMapModel::ItemMapModel(QObject *parent,
                           QMap<ItemType, QList<std::shared_ptr<Item> > > initialContainer,
                           QList<ItemType> toShow,
                           bool showPrice)
    : QAbstractTableModel(parent),
      _container(initialContainer),
      _showPrice(showPrice)
{
    if (toShow.empty()) {
        _toShow = QList<ItemType>();
        for (int i = 0; i < int(LAST_ITEM_TYPE); ++i) {
            _toShow.push_back(ItemType(i));
        }
    } else {
        _toShow = toShow;
    }
}

std::shared_ptr<Item> ItemMapModel::getItemFromIndex(const QModelIndex& index) const {
    int i = 0;
    for (const auto& itemType : _container.keys()) {
        if (!_toShow.contains(itemType))
            continue;
        const auto& itemGroup = _container[itemType];
        if (index.row() < i + itemGroup.size()) {
            return itemGroup[index.row() - i];
        } else {
            i += itemGroup.size();
        }
    }
    return nullptr;
}

int ItemMapModel::rowCount(const QModelIndex &/*parent*/) const {
    int size = 0;
    for (const auto& itemType : _container.keys()) {
        if (_toShow.contains(itemType))
            size += _container[itemType].size();
    }
    return size;
}

int ItemMapModel::columnCount(const QModelIndex &/*parent*/) const {
    return _showPrice ? 3 : 2;
}

QVariant ItemMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Предмет");
        case 1:
            return QString("Количество");
        case 2:
            return QString("Цена");
        }
    }
    return QVariant();
}

QVariant ItemMapModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
        std::shared_ptr<Item> item = getItemFromIndex(index);
        if (role == Qt::DisplayRole){
            switch (index.column()) {
            case 0:
                return item->name;
            case 1:
                return item->amount;
            case 2:
                return item->price;
            }
        } else {
            return item->description;
        }
    }
    return QVariant();
}

void ItemMapModel::updateContainer(const QMap<ItemType, QList<std::shared_ptr<Item>>>& map) {
    _container = map;
    emit modelReset({});
}

InventoryItemModel::InventoryItemModel(QObject* parent, std::shared_ptr<Game> game, QList<ItemType> toShow, bool showPrice) :
    ItemMapModel(parent, game->_hero->inventory(), toShow, showPrice) {
    QObject::connect(&(*game->_hero),
                         SIGNAL(inventory_changed(const QMap<ItemType, QList<std::shared_ptr<Item>>>&)),
                        this,
                        SLOT(updateContainer(const QMap<ItemType, QList<std::shared_ptr<Item>>>&)));

}

ShopItemModel::ShopItemModel(QObject* parent, std::shared_ptr<Game> game) :
    ItemMapModel(parent, game->_shop->globalShop(), {}, true), _hero(game->_hero), _shop(game->_shop) {
    QObject::connect(&(*_shop),
                         SIGNAL(shopChanged(const QMap<ItemType, QList<std::shared_ptr<Item>>>&)),
                        this,
                        SLOT(updateContainer(const QMap<ItemType, QList<std::shared_ptr<Item>>>&)));
}

void ShopItemModel::onTableClicked(const QModelIndex& index) {
    if (index.isValid()) {
        std::shared_ptr<Item> item = getItemFromIndex(index);
        _shop->shopToHeroTrade(_hero, item);
    }
}

SellItemModel::SellItemModel(QObject* parent, std::shared_ptr<Game> game) :
    InventoryItemModel(parent, game, {}, 1), _hero(game->_hero), _shop(game->_shop) {

}

void SellItemModel::onTableClicked(const QModelIndex& index) {
    if (index.isValid()) {
        std::shared_ptr<Item> item = getItemFromIndex(index);
        _shop->heroToShopTrade(_hero, item);
    }
}

CharacteristicsItemModel::CharacteristicsItemModel(QObject *parent,
                                                   std::shared_ptr<Game> game) :
    InventoryItemModel(parent, game, {
                       ItemType::SHIP_CONSUMABLE,
                       ItemType::SHIP_CANNONS,
                       ItemType::SHIP_HULL,
                       ItemType::SHIP_SAIL}, false),
    _hero(game->_hero)
{

}

void CharacteristicsItemModel::onTableClicked(const QModelIndex& index) {
    if (index.isValid()) {
        std::shared_ptr<Item> item = getItemFromIndex(index);
        switch (item->getType()) {
        case ItemType::SHIP_HULL:
        case ItemType::SHIP_SAIL:
        case ItemType::SHIP_CANNONS:
            std::dynamic_pointer_cast<ShipEquipment>(item)->equip(&*_hero);
            break;
        default:
            break;
        }

    }
}
