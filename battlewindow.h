#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QDialog>

namespace Ui {
class BattleWindow;
}

class BattleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BattleWindow(QWidget *parent = nullptr);
    ~BattleWindow();

private:
    Ui::BattleWindow *ui;
};

#endif // BATTLEWINDOW_H
