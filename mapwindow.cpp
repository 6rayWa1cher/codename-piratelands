#include "mapwindow.h"
#include "ui_mapwindow.h"

MapWindow::MapWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&_scene);
    _scene.addPixmap(QPixmap(":/resources/map.png"));
}

MapWindow::~MapWindow()
{
    delete ui;
}
