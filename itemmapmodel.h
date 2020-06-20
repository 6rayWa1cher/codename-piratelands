#ifndef ITEMMAPMODEL_H
#define ITEMMAPMODEL_H

#include <QAbstractTableModel>
#include "item.h"
#include "game.h"

class ItemMapModel : public QAbstractTableModel {
    Q_OBJECT
protected:
    QMap<ItemType, QList<std::shared_ptr<Item>>> _container;
    QList<ItemType> _toShow;
    bool _showPrice;
public:
    ItemMapModel(QObject* parent,
                 QMap<ItemType, QList<std::shared_ptr<Item>>> initialContainer,
                 QList<ItemType> toShow,
                 bool showPrice);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
protected:
    std::shared_ptr<Item> getItemFromIndex(const QModelIndex& index) const;
public slots:
    void updateContainer(const QMap<ItemType, QList<std::shared_ptr<Item>>>& map);
};

class ShopItemModel : public ItemMapModel {
    Q_OBJECT
protected:
    std::shared_ptr<Hero> _hero;
    std::shared_ptr<Shop> _shop;
public:
    ShopItemModel(QObject* parent, std::shared_ptr<Game> game);
public slots:
    void onTableClicked(const QModelIndex& index);
};

class InventoryItemModel : public ItemMapModel {
    Q_OBJECT
public:
    InventoryItemModel(QObject* parent, std::shared_ptr<Game> game, QList<ItemType> toShow, bool showPrice);
};

class SellItemModel : public InventoryItemModel {
    Q_OBJECT
protected:
    std::shared_ptr<Hero> _hero;
    std::shared_ptr<Shop> _shop;
public:
    SellItemModel(QObject* parent, std::shared_ptr<Game> game);
public slots:
    void onTableClicked(const QModelIndex& index);
};

class CharacteristicsItemModel : public InventoryItemModel {
    Q_OBJECT
protected:
    std::shared_ptr<Hero> _hero;
public:
    CharacteristicsItemModel(QObject* parent, std::shared_ptr<Game> game);
public slots:
    void onTableClicked(const QModelIndex& index);
};

#endif // ITEMMAPMODEL_H
