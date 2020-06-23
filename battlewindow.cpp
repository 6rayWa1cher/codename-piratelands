#include "battlewindow.h"
#include "ui_battlewindow.h"
#include "game.h"
#include <QScrollBar>
#include <QCloseEvent>

BattleWindow::BattleWindow(QWidget *parent, std::shared_ptr<Game> game) :
    QDialog(parent),
    _game(game),
    _bww(this),
    _model(this, game),
    ui(new Ui::BattleWindow)
{
    ui->setupUi(this);
    ui->inventory->setModel(&_model);
    connect(ui->inventory, SIGNAL(doubleClicked(const QModelIndex &)), &_model, SLOT(onTableClicked(const QModelIndex &)));
    QObject::connect(&*game->_world, SIGNAL(encounter(EncounterType, std::shared_ptr<Enemy>)),
                     this, SLOT(startEncount(EncounterType, std::shared_ptr<Enemy>)));
    QObject::connect(&*game->_battle, SIGNAL(battleEvent(QString)), this, SLOT(addToLog(QString)));
    QObject::connect(ui->attack_button, SIGNAL(clicked()), &*game->_battle, SLOT(attack()));
    QObject::connect(ui->boarding_button, SIGNAL(clicked()), &*game->_battle, SLOT(boarding()));
    QObject::connect(ui->escape_button, SIGNAL(clicked()), &*game->_battle, SLOT(fleeing()));
    QObject::connect(&*game->_hero, SIGNAL(health_changed(uint16_t)), this, SLOT(heroHealthChanged(uint16_t)));
    QObject::connect(&*game->_battle, SIGNAL(battleOver(std::shared_ptr<Enemy>, BattleWonResult)), this, SLOT(battleIsOver(std::shared_ptr<Enemy>, BattleWonResult)));
    QObject::connect(&_bww, SIGNAL(accepted()), this, SLOT(fullEndBattle()));
}

BattleWindow::~BattleWindow()
{
    delete ui;
}

void BattleWindow::closeEvent(QCloseEvent * event)
{
    event->ignore();
}

void BattleWindow::updateUi()
{
    std::shared_ptr<Enemy> enemy = _game->_battle->currentEnemy();
    std::shared_ptr<Hero> hero = _game->_hero;
    ui->enemy_label->setText(enemy->name());
    ui->enemy_health->setRange(0, enemy->maxHealth());
    ui->enemy_health->setValue(enemy->health());
    ui->hero_label->setText(hero->name());
    ui->hero_health->setRange(0, hero->maxHealth());
    ui->hero_health->setValue(hero->health());
}

void BattleWindow::fullEndBattle()
{
    hide();
    setDisabled(false);
    emit battleReleased();
}

void BattleWindow::addToLog(QString str)
{
    const QString& text = ui->log->toPlainText();
    if (text == "") {
        ui->log->setText(str);
    } else {
        ui->log->setText(text + "\r\n" + str);
    }
    ui->log->verticalScrollBar()->setValue(ui->log->verticalScrollBar()->maximum());
}

void BattleWindow::startEncount(EncounterType /*type*/, std::shared_ptr<Enemy> enemy)
{
    QObject::connect(&*enemy, SIGNAL(health_changed(uint16_t)), this, SLOT(enemyHealthChanged(uint16_t)));
    _game->_battle->startBattle(enemy);
    ui->log->setText("");
    updateUi();
    this->show();
}

void BattleWindow::battleIsOver(std::shared_ptr<Enemy> enemy, BattleWonResult result)
{
    QObject::disconnect(&*enemy, SIGNAL(health_changed(uint16_t)), this, SLOT(enemyHealthChanged(uint16_t)));
//    this->hide();
    if (result.gold > 0){
        setDisabled(true);
        _bww.show(result);
    } else {
        fullEndBattle();
    }
}

//void BattleWindow::acceptedItems()
//{
//    fullEndBattle();
//}

void BattleWindow::heroHealthChanged(uint16_t value)
{
    ui->hero_health->setValue(value);
}

void BattleWindow::enemyHealthChanged(uint16_t value)
{
    ui->enemy_health->setValue(value);
}

void BattleWindow::heroMaxHealthChanged(uint16_t value)
{
    ui->hero_health->setRange(0, value);
}
