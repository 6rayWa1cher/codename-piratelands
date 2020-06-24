#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "battlewindow.h"
#include "enemystatsmodel.h"
#include "game.h"
#include "shopwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, ShopWindow* _shopWindow = nullptr,
               std::shared_ptr<Game> game = nullptr);
    ~MainWindow();

private:
    ShopWindow* _shopWindow;
    BattleWindow* _battleWindow;
    InventoryItemModel _worldInventoryModel;
    CharacteristicsItemModel _characteristicsInventoryModel;
    EnemyStatsModel _heroStatsModel;
    Ui::MainWindow *ui;
    std::shared_ptr<Game> _game;
public slots:
    void rerenderCurrentRoom();
    void presentNewItem(std::shared_ptr<Item> item);
    void enterRoom(int room);
    void moveNorth();
    void moveEast();
    void moveWest();
    void moveSouth();
    void displayShop();
    void updateHeroHealth(uint16_t health);
    void updateHeroMaxHealth(uint16_t health);
    void startEncount(EncounterType type, std::shared_ptr<Enemy> enemy);
    void encountEnd(std::shared_ptr<Enemy> enemy, BattleWonResult result);
    void setHeroName(QString name);
    void moneyChanged(int money);
};
#endif // MAINWINDOW_H
