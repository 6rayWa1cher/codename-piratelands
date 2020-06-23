#include "battlewonwindow.h"
#include "ui_battlewonwindow.h"
#include "battlewindow.h"

BattleWonWindow::BattleWonWindow(BattleWindow* battleWindow) :
    QDialog(battleWindow),
    ui(new Ui::BattleWonWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(acceptItems()));
}

BattleWonWindow::~BattleWonWindow()
{
    delete ui;
}

void BattleWonWindow::show(BattleWonResult result)
{
    ui->item1->setText(QString("Золото: ") + std::to_string(result.gold).c_str());
    if (result.item) {
        ui->item2->show();
        if (result.selled) {
            ui->item2->setText(QString("Золото: ") + std::to_string(result.item->price).c_str() + " (трофей продан)");
        } else {
            ui->item2->setText(result.item->name);
            ui->item2->setToolTip(result.item->description);
        }
    } else {
        ui->item2->hide();
    }
    setDisabled(false);
    QDialog::show();
}

void BattleWonWindow::acceptItems()
{
    hide();
    emit accepted();
}
