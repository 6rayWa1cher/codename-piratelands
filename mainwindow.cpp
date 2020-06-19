#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, std::shared_ptr<Game> game)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _game(game) {
    ui->setupUi(this);
    enterRoom(0);
    connect(&(*game->_hero), SIGNAL(hero_moved(int)), this, SLOT(enterRoom(int)));
    connect(ui->north, SIGNAL(clicked()), this, SLOT(moveNorth()));
    connect(ui->east, SIGNAL(clicked()), this, SLOT(moveEast()));
    connect(ui->west, SIGNAL(clicked()), this, SLOT(moveWest()));
    connect(ui->south, SIGNAL(clicked()), this, SLOT(moveSouth()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::enterRoom(int room) {
    auto _room = (*(*_game)._world)[room];
    ui->zone_description->setText(_room._name + "\n\n" + _room._description);
    auto map = _room._neighbourRooms;
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

