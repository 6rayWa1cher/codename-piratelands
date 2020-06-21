#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "item.h"


MainWindow::MainWindow(QWidget *parent, ShopWindow* shopWindow, std::shared_ptr<Game> game)
    : QMainWindow(parent)
    , _shopWindow(shopWindow)
    , _worldInventoryModel(this, game, {ItemType::KEY_ITEM}, false)
    , _characteristicsInventoryModel(this, game)
    , ui(new Ui::MainWindow)
    , _game(game){
    ui->setupUi(this);
    enterRoom(15);
    ui->inventory->setModel(&_worldInventoryModel);
    ui->characteristics_inventorty_view->setModel(&_characteristicsInventoryModel);
    connect(ui->characteristics_inventorty_view, SIGNAL(doubleClicked(const QModelIndex &)), &_characteristicsInventoryModel, SLOT(onTableClicked(const QModelIndex &)));
    connect(&(*game->_hero), SIGNAL(hero_moved(int)), this, SLOT(enterRoom(int)));
    connect(ui->north, SIGNAL(clicked()), this, SLOT(moveNorth()));
    connect(ui->east, SIGNAL(clicked()), this, SLOT(moveEast()));
    connect(ui->west, SIGNAL(clicked()), this, SLOT(moveWest()));
    connect(ui->south, SIGNAL(clicked()), this, SLOT(moveSouth()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(displayShop()));

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::enterRoom(int room) {
    auto _room = (*(*_game)._world)[room];
    ui->zone_description->setText(_room->name() + "\n\n" + _room->description());
    auto map = _room->_neighbourRooms;
    ui->north->setEnabled(map[Direction::North] != -1);
    ui->east->setEnabled(map[Direction::East] != -1);
    ui->west->setEnabled(map[Direction::West] != -1);
    ui->south->setEnabled(map[Direction::South] != -1);
}

void MainWindow::moveNorth() {
    _game->_hero->move(Direction::North);
}

void MainWindow::moveEast() {
    _game->_hero->move(Direction::East);
}

void MainWindow::moveWest() {
    _game->_hero->move(Direction::West);
}

void MainWindow::moveSouth() {
    _game->_hero->move(Direction::South);
}

void MainWindow::displayShop() {
    _shopWindow->show();
}
