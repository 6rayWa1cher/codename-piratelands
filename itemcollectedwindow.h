#ifndef ITEMCOLLECTEDWINDOW_H
#define ITEMCOLLECTEDWINDOW_H

#include <QDialog>
#include "item.h"

namespace Ui {
class ItemCollectedWindow;
}

class ItemCollectedWindow : public QDialog {
    Q_OBJECT
public:
    ItemCollectedWindow(QWidget *parent = nullptr, std::shared_ptr<Item> item = nullptr);
    ~ItemCollectedWindow();

private:
    Ui::ItemCollectedWindow *ui;
};

#endif // ITEMCOLLECTEDWINDOW_H
