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
    ui->money_lcd->display(std::to_string(game->_hero->money()).c_str());
    ui->shopTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->shopTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->shopTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->shopTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->playerTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->playerTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->playerTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->playerTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    QObject::connect(ui->shopTable, SIGNAL(doubleClicked(const QModelIndex &)), &_shopItemModel, SLOT(onTableClicked(const QModelIndex &)));
    QObject::connect(ui->playerTable, SIGNAL(doubleClicked(const QModelIndex &)), &_sellItemModel, SLOT(onTableClicked(const QModelIndex &)));
    QObject::connect(&*game->_hero, SIGNAL(money_changed(int)), this, SLOT(moneyChanged(int)));
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
}

ShopWindow::~ShopWindow() {
    delete ui;
}

void ShopWindow::moneyChanged(int money) {
    ui->money_lcd->display(money);
}
