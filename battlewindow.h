#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include "battle.h"
#include "battlewonwindow.h"
#include "itemmapmodel.h"

#include <QDialog>

namespace Ui {
class BattleWindow;
}

class BattleWindow : public QDialog
{
    Q_OBJECT
private:
    std::shared_ptr<Game> _game;
    BattleWonWindow _bww;
    BattleItemModel _model;
    Ui::BattleWindow *ui;
public:
    BattleWindow(QWidget *parent = nullptr,
                 std::shared_ptr<Game> game = nullptr);
    ~BattleWindow();
    void closeEvent(QCloseEvent * event) override;
private:
    void updateUi();
signals:
    void battleReleased();
public slots:
    void fullEndBattle();
    void startEncount(EncounterType type, std::shared_ptr<Enemy> enemy);
    void battleIsOver(std::shared_ptr<Enemy> enemy, BattleWonResult result);
    void addToLog(QString str);
    void heroHealthChanged(uint16_t value);
    void enemyHealthChanged(uint16_t value);
    void heroMaxHealthChanged(uint16_t value);
};

#endif // BATTLEWINDOW_H
