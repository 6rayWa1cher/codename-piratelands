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
    connect(ui->shop_button, SIGNAL(clicked()), this, SLOT(displayShop()));
    connect(&(*game->_hero), SIGNAL(team_changed(std::shared_ptr<ShipBoardingTeam>)), this, SLOT(replaceTeam(std::shared_ptr<ShipBoardingTeam>)));
    connect(&(*game->_hero), SIGNAL(cannons_changed(std::shared_ptr<ShipCannons>)), this, SLOT(replaceCannons(std::shared_ptr<ShipCannons>)));
    connect(&(*game->_hero), SIGNAL(hull_changed(std::shared_ptr<ShipHull>)), this, SLOT(replaceHull(std::shared_ptr<ShipHull>)));
    connect(&(*game->_hero), SIGNAL(sail_changed(std::shared_ptr<ShipSail>)), this, SLOT(replaceSail(std::shared_ptr<ShipSail>)));
    connect(&(*game->_world), SIGNAL(worldChanged()), this, SLOT(rerenderCurrentRoom()));
    connect(&(*game->_hero), SIGNAL(health_changed(uint16_t)), this, SLOT(updateHeroHealth(uint16_t)));
    connect(&(*game->_hero), SIGNAL(max_health_changed(uint16_t)), this, SLOT(updateHeroMaxHealth(uint16_t)));
    connect(&(*game->_battle), SIGNAL(battleOver(std::shared_ptr<Enemy>, BattleWonResult)), this, SLOT(encountEnd(std::shared_ptr<Enemy>, BattleWonResult)));
    connect(&(*game->_world), SIGNAL(encounter(EncounterType, std::shared_ptr<Enemy>)), this, SLOT(startEncount(EncounterType, std::shared_ptr<Enemy>)));
    replaceHull(game->_hero->hull());
    replaceSail(game->_hero->sail());
    replaceTeam(game->_hero->team());
    replaceCannons(game->_hero->cannons());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::rerenderCurrentRoom()
{
    enterRoom(_game->_hero->currentRoom());
}

void MainWindow::presentNewItem(std::shared_ptr<Item> item)
{
    ItemCollectedWindow w(this, item);
    w.show();
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
    _shopWindow->show();
}

void MainWindow::replaceTeam(std::shared_ptr<ShipBoardingTeam> team)
{
    ui->team_name->setText(team->name);
    ui->team_name->setToolTip(team->description);
}

void MainWindow::replaceCannons(std::shared_ptr<ShipCannons> cannons) {
    ui->equipped_melee_weapon_name->setText(cannons->name);
    ui->equipped_melee_weapon_name->setToolTip(cannons->description);
}

void MainWindow::replaceHull(std::shared_ptr<ShipHull> hull) {
    ui->equipped_range_weapon_name->setText(hull->name);
    ui->equipped_range_weapon_name->setToolTip(hull->description);
}

void MainWindow::replaceSail(std::shared_ptr<ShipSail> sail) {
    ui->equipped_armor_name->setText(sail->name);
    ui->equipped_armor_name->setToolTip(sail->description);
}

void MainWindow::updateHeroHealth(uint16_t health) {
    ui->health_bar->setValue(int(health));
}

void MainWindow::updateHeroMaxHealth(uint16_t health) {
    ui->health_bar->setMaximum(health);

}

void MainWindow::startEncount(EncounterType /*type*/, std::shared_ptr<Enemy> /*enemy*/)
{
    this->setDisabled(true);
}

void MainWindow::encountEnd(std::shared_ptr<Enemy> /*enemy*/, BattleWonResult /*result*/)
{
    this->setDisabled(false);
}

