#ifndef SHOP_H
#define SHOP_H

#include <QObject>

#include "item.h"
#include "hero.h"

class Shop : public QObject
{
    Q_OBJECT
private:
    QMap<ItemType, QList<std::shared_ptr<Item>>> _globalShop;
public:
    Shop(QObject *parent = nullptr);
    bool shopToHeroTrade(std::shared_ptr<Hero> hero, std::shared_ptr<Item> item);
    bool heroToShopTrade(std::shared_ptr<Hero> hero, std::shared_ptr<Item> item);
    void addItem(std::shared_ptr<Item> item);
    void removeItem(std::shared_ptr<Item> item);

    QMap<ItemType, QList<std::shared_ptr<Item> > > globalShop() const;

signals:
    void shopChanged(const QMap<ItemType, QList<std::shared_ptr<Item>>>& inventory);
};

#endif // SHOP_H
