#include "battlelostwindow.h"
#include "ui_battlelostwindow.h"

battlelostwindow::battlelostwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::battlelostwindow)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(_continue()));
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
}

battlelostwindow::~battlelostwindow() {
    delete ui;
}

void battlelostwindow::_continue() {
    hide();
    emit accepted();
}
