#include "copyrightwindow.h"
#include "ui_copyrightwindow.h"

CopyrightWindow::CopyrightWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyrightWindow)
{
    ui->setupUi(this);
}

CopyrightWindow::~CopyrightWindow()
{
    delete ui;
}
