#include "shopwindow.h"
#include "ui_shopwindow.h"


ShopWindow::ShopWindow(QWidget *parent, std::shared_ptr<Game> game) :
    QDialog(parent),
    _shopItemModel(this, game),
    _sellItemModel(this, game),
    ui(new Ui::ShopWindow)
{
    ui->setupUi(this);
    ui->shopTable->setModel(&_shopItemModel);
    ui->playerTable->setModel(&_sellItemModel);
    QObject::connect(ui->shopTable, SIGNAL(doubleClicked(const QModelIndex &)), &_shopItemModel, SLOT(onTableClicked(const QModelIndex &)));
    QObject::connect(ui->playerTable, SIGNAL(doubleClicked(const QModelIndex &)), &_sellItemModel, SLOT(onTableClicked(const QModelIndex &)));
}

ShopWindow::~ShopWindow()
{
    delete ui;
}
