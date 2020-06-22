#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include "shopwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, ShopWindow* _shopWindow = nullptr, std::shared_ptr<Game> game = nullptr);
    ~MainWindow();

private:
    ShopWindow* _shopWindow;
    InventoryItemModel _worldInventoryModel;
    CharacteristicsItemModel _characteristicsInventoryModel;
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
    void replaceCannons(std::shared_ptr<ShipCannons> cannons);
    void replaceHull(std::shared_ptr<ShipHull> hull);
    void replaceSail(std::shared_ptr<ShipSail> sail);
};
#endif // MAINWINDOW_H
