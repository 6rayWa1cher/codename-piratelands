#ifndef BATTLEWONWINDOW_H
#define BATTLEWONWINDOW_H

#include "item.h"
#include "battle.h"

#include <QDialog>

class BattleWindow;

namespace Ui {
class BattleWonWindow;
}

class BattleWonWindow : public QDialog
{
    Q_OBJECT
private:
    Ui::BattleWonWindow *ui;
public:
    BattleWonWindow(BattleWindow* battleWindow);
    ~BattleWonWindow();
    void show(BattleWonResult result);
signals:
    void accepted();
public slots:
    void acceptItems();
};

#endif // BATTLEWONWINDOW_H
