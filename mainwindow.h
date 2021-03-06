#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include "battlewindow.h"
#include "enemystatsmodel.h"
#include "itemcollectedwindow.h"
#include "game.h"
#include "shopwindow.h"
#include "mapwindow.h"
#include "copyrightwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, ShopWindow* _shopWindow = nullptr, MapWindow* mapWindow = nullptr, CopyrightWindow* copyrightWindow = nullptr,
               std::shared_ptr<Game> game = nullptr);
    ~MainWindow();

private:
    ShopWindow* _shopWindow;
    ItemCollectedWindow* _itemCollectedWindow;
    MapWindow* _mapWindow;
    CopyrightWindow* _copyrightWindow;
    InventoryItemModel _worldInventoryModel;
    CharacteristicsItemModel _characteristicsInventoryModel;
    QGraphicsScene _scene;
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
    void heroInventoryChanged(const QMap<ItemType, QList<std::shared_ptr<Item>>>& inventory);
    void setHeroName(QString name);
    void moneyChanged(int money);
    void replaceTeam(std::shared_ptr<ShipBoardingTeam> team);
    void replaceCannons(std::shared_ptr<ShipCannons> cannons);
    void replaceHull(std::shared_ptr<ShipHull> hull);
    void replaceSail(std::shared_ptr<ShipSail> sail);
    void updateStats();
};
#endif // MAINWINDOW_H
