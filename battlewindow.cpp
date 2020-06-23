#include "battlewindow.h"
#include "ui_battlewindow.h"

BattleWindow::BattleWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BattleWindow)
{
    ui->setupUi(this);
}

BattleWindow::~BattleWindow()
{
    delete ui;
}
