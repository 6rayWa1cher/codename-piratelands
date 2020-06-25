#include "battlewindow.h"
#include "ui_battlewindow.h"
#include "game.h"
#include <QScrollBar>
#include <QCloseEvent>

BattleWindow::BattleWindow(QWidget *parent, std::shared_ptr<Game> game) :
    QDialog(parent),
    _game(game),
    _bww(this),
    _blw(),
    _model(this, game),
    _heroStatsModel(this, game->_hero),
    _enemyStatsModel(this, game->_hero),
    ui(new Ui::BattleWindow)
{
    _bww.setFixedSize(_bww.size());
    _blw.setFixedSize(_blw.size());
    _bww.setWindowTitle("Победа!");
    _blw.setWindowTitle("Поражение!");
    ui->setupUi(this);
    ui->inventory->setModel(&_model);
    ui->hero_slots->setModel(&_heroStatsModel);
    ui->enemy_slots->setModel(&_enemyStatsModel);
    ui->hero_slots->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->hero_slots->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->hero_slots->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->enemy_slots->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->enemy_slots->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->enemy_slots->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->inventory->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->inventory->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->inventory->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    QObject::connect(ui->inventory, SIGNAL(doubleClicked(const QModelIndex &)), &_model, SLOT(onTableClicked(const QModelIndex &)));
    QObject::connect(&*game->_world, SIGNAL(encounter(EncounterType, std::shared_ptr<Enemy>)),
                     this, SLOT(startEncount(EncounterType, std::shared_ptr<Enemy>)));
    QObject::connect(&*game->_battle, SIGNAL(battleEvent(QString)), this, SLOT(addToLog(QString)));
    QObject::connect(ui->attack_button, SIGNAL(clicked()), &*game->_battle, SLOT(attack()));
    QObject::connect(ui->boarding_button, SIGNAL(clicked()), &*game->_battle, SLOT(boarding()));
    QObject::connect(ui->escape_button, SIGNAL(clicked()), &*game->_battle, SLOT(fleeing()));
    QObject::connect(&*game->_hero, SIGNAL(health_changed(uint16_t)), this, SLOT(heroHealthChanged(uint16_t)));
    QObject::connect(&*game->_battle, SIGNAL(battleOver(std::shared_ptr<Enemy>, BattleWonResult)), this, SLOT(battleIsOver(std::shared_ptr<Enemy>, BattleWonResult)));
    QObject::connect(&_bww, SIGNAL(accepted()), this, SLOT(fullEndBattle()));
    QObject::connect(&_blw, SIGNAL(accepted()), this, SLOT(fullEndBattle()));
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
}

BattleWindow::~BattleWindow() {
    delete ui;
}

void BattleWindow::closeEvent(QCloseEvent * event) {
    event->ignore();
}

void BattleWindow::updateUi() {
    std::shared_ptr<Enemy> enemy = _game->_battle->currentEnemy();
    std::shared_ptr<Hero> hero = _game->_hero;
    ui->enemy_label->setText(enemy->name());
    ui->enemy_health->setRange(0, enemy->maxHealth());
    ui->enemy_health->setValue(enemy->health());
    ui->hero_label->setText(hero->name());
    ui->hero_health->setRange(0, hero->maxHealth());
    ui->hero_health->setValue(hero->health());
}

void BattleWindow::fullEndBattle() {
    hide();
    setDisabled(false);
    emit battleReleased();
}

void BattleWindow::addToLog(QString str) {
    const QString& text = ui->log->toPlainText();
    if (text == "") {
        ui->log->setText(str);
    } else {
        ui->log->setText(text + "\r\n" + str);
    }
    ui->log->verticalScrollBar()->setValue(ui->log->verticalScrollBar()->maximum());
}

void BattleWindow::startEncount(EncounterType /*type*/, std::shared_ptr<Enemy> enemy) {
    QObject::connect(&*enemy, SIGNAL(health_changed(uint16_t)), this, SLOT(enemyHealthChanged(uint16_t)));
    _game->_battle->startBattle(enemy);
    ui->log->setText("");
    _enemyStatsModel.setEnemy(enemy);
    updateUi();
    this->setWindowTitle("Бой");
    this->show();
}

void BattleWindow::battleIsOver(std::shared_ptr<Enemy> enemy, BattleWonResult result) {
    QObject::disconnect(&*enemy, SIGNAL(health_changed(uint16_t)), this, SLOT(enemyHealthChanged(uint16_t)));
//    this->hide();
    _enemyStatsModel.setEnemy(_game->_hero);
    if (result.gold > 0){
        setDisabled(true);
        _bww.show(result);
    } else {
        _blw.show();
        fullEndBattle();
    }
}

//void BattleWindow::acceptedItems()
//{
//    fullEndBattle();
//}

void BattleWindow::heroHealthChanged(uint16_t value) {
    ui->hero_health->setValue(value);
}

void BattleWindow::enemyHealthChanged(uint16_t value) {
    ui->enemy_health->setValue(value);
}

void BattleWindow::heroMaxHealthChanged(uint16_t value) {
    ui->hero_health->setRange(0, value);
}
