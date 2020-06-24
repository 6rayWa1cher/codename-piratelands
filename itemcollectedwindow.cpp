#include "itemcollectedwindow.h"
#include "ui_itemcollectedwindow.h"

ItemCollectedWindow::ItemCollectedWindow(QWidget *parent, std::shared_ptr<Item> item) :
    QDialog(parent),
    ui(new Ui::ItemCollectedWindow)
{
    ui->setupUi(this);
    ui->item_name->setText(item->name);
}

ItemCollectedWindow::~ItemCollectedWindow() {
    delete ui;
}

