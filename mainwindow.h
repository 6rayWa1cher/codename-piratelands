#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, std::shared_ptr<Game> game = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Game> _game;
public slots:
    void enterRoom(int room);
    void moveNorth();
    void moveEast();
    void moveWest();
    void moveSouth();

};
#endif // MAINWINDOW_H
