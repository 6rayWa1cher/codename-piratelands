#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "item.h"
#include "itemcollectedwindow.h"
#include <iostream>


MainWindow::MainWindow(QWidget *parent, ShopWindow* shopWindow, std::shared_ptr<Game> game)
    : QMainWindow(parent)
    , _shopWindow(shopWindow)
    , _worldInventoryModel(this, game, {ItemType::KEY_ITEM}, false)
    , _characteristicsInventoryModel(this, game)
    , _heroStatsModel(this, game->_hero)
    , ui(new Ui::MainWindow)
    , _game(game){
    ui->setupUi(this);
    enterRoom(15);
    ui->inventory->setModel(&_worldInventoryModel);
    ui->characteristics_inventorty_view->setModel(&_characteristicsInventoryModel);
    ui->hero_slots->setModel(&_heroStatsModel);
    ui->hero_slots->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->characteristics_inventorty_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->characteristics_inventorty_view->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->characteristics_inventorty_view->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->hero_slots->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->hero_slots->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->hero_slots->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->inventory->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->inventory->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->inventory->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->acc_value->setSegmentStyle(QLCDNumber::SegmentStyle::Filled);
    connect(ui->characteristics_inventorty_view, SIGNAL(doubleClicked(const QModelIndex &)), &_characteristicsInventoryModel, SLOT(onTableClicked(const QModelIndex &)));
    connect(&(*game->_hero), SIGNAL(hero_moved(int)), this, SLOT(enterRoom(int)));
    connect(ui->north, SIGNAL(clicked()), this, SLOT(moveNorth()));
    connect(ui->east, SIGNAL(clicked()), this, SLOT(moveEast()));
    connect(ui->west, SIGNAL(clicked()), this, SLOT(moveWest()));
    connect(ui->south, SIGNAL(clicked()), this, SLOT(moveSouth()));
    connect(ui->shop_button, SIGNAL(clicked()), this, SLOT(displayShop()));
    connect(&(*game->_world), SIGNAL(worldChanged()), this, SLOT(rerenderCurrentRoom()));
    connect(&(*game->_hero), SIGNAL(health_changed(uint16_t)), this, SLOT(updateHeroHealth(uint16_t)));
    connect(&(*game->_hero), SIGNAL(max_health_changed(uint16_t)), this, SLOT(updateHeroMaxHealth(uint16_t)));
    connect(&(*game->_battle), SIGNAL(battleOver(std::shared_ptr<Enemy>, BattleWonResult)), this, SLOT(encountEnd(std::shared_ptr<Enemy>, BattleWonResult)));
    connect(&(*game->_world), SIGNAL(encounter(EncounterType, std::shared_ptr<Enemy>)), this, SLOT(startEncount(EncounterType, std::shared_ptr<Enemy>)));
    connect(&_characteristicsInventoryModel, SIGNAL(stat_changed()), this, SLOT(updateStats()));
    QObject::connect(&*game->_hero, SIGNAL(money_changed(int)), this, SLOT(moneyChanged(int)));
    connect(&(*(game->_world)), SIGNAL(loudAddItem(std::shared_ptr<Item>)), this, SLOT(presentNewItem(std::shared_ptr<Item>)));
    replaceHull(game->_hero->hull());
    replaceSail(game->_hero->sail());
    replaceTeam(game->_hero->team());
    replaceCannons(game->_hero->cannons());
    setHeroName(game->_hero->name());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::rerenderCurrentRoom() {
    enterRoom(_game->_hero->currentRoom());
}

void MainWindow::presentNewItem(std::shared_ptr<Item> item) {
    ItemCollectedWindow* iw = new ItemCollectedWindow(this, item);
    iw->setWindowTitle("Оповещение!");
    iw->show();
}

void MainWindow::enterRoom(int room) {
    auto _room = (*(*_game)._world)[room];
    ui->zone_description->setText(_room->name() + "\n\n" + _room->description());
    ui->shop_button->setEnabled(_room->isShopAvailable());
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
    _shopWindow->setWindowTitle("Магазин");
    _shopWindow->show();
}

void MainWindow::replaceTeam(std::shared_ptr<ShipBoardingTeam> team) {
    ui->bp_value->display(int(team->baseBoardingPower * (100 - _game->_hero->hull()->boardingPowerDecreasement) / 100));
}

void MainWindow::replaceCannons(std::shared_ptr<ShipCannons> cannons) {
    ui->atk_value->display(int(cannons->baseAttack));
    ui->acc_value->display(int(cannons->baseAccuracy));
}

void MainWindow::replaceHull(std::shared_ptr<ShipHull> hull) {
    ui->arm_value->display(int(hull->baseArmor));
    ui->wc_value->display(int(hull->baseCarryingCapacity));
    updateHeroMaxHealth(_game->_hero->maxHealth());
    updateHeroHealth(_game->_hero->health());
}

void MainWindow::replaceSail(std::shared_ptr<ShipSail> sail) {
    ui->ev_value->display(int(sail->baseEvasion * (100 - _game->_hero->hull()->evasionDecreasement) / 100));
    ui->esc_value->display(int(sail->baseEscape * (100 - _game->_hero->hull()->escapeDecreasement) / 100));
}

void MainWindow::updateStats() {
    auto hero = _game->_hero;
    replaceHull(hero->hull());
    replaceSail(hero->sail());
    replaceTeam(hero->team());
    replaceCannons(hero->cannons());
}

void MainWindow::updateHeroHealth(uint16_t health) {
    ui->health_bar->setValue(health);
}

void MainWindow::updateHeroMaxHealth(uint16_t health) {
    ui->health_bar->setMaximum(health);
}

void MainWindow::startEncount(EncounterType /*type*/, std::shared_ptr<Enemy> /*enemy*/) {
    this->setDisabled(true);
}

void MainWindow::encountEnd(std::shared_ptr<Enemy> /*enemy*/, BattleWonResult /*result*/) {
    this->setDisabled(false);
}

void MainWindow::setHeroName(QString name) {
    ui->captain_label->setText("Капитан " + name);
}

void MainWindow::moneyChanged(int money) {
    ui->money_display->display(money);
}
