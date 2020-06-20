#ifndef SHOPWINDOW_H
#define SHOPWINDOW_H

#include "game.h"
#include "itemmapmodel.h"

#include <QAbstractTableModel>
#include <QDialog>

namespace Ui {
class ShopWindow;
}

class ShopWindow : public QDialog
{
    Q_OBJECT
protected:
    std::shared_ptr<Hero> _hero;
public:
    ShopWindow(QWidget *parent = nullptr, std::shared_ptr<Game> game = nullptr);
    ~ShopWindow();
private:
    ShopItemModel _shopItemModel;
    SellItemModel _sellItemModel;
    Ui::ShopWindow *ui;
};

#endif // SHOPWINDOW_H
