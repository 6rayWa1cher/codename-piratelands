#include "itemcollectedwindow.h"
#include "ui_itemcollectedwindow.h"

itemcollectedwindow::itemcollectedwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::itemcollectedwindow)
{
    ui->setupUi(this);
}

itemcollectedwindow::~itemcollectedwindow()
{
    delete ui;
}
