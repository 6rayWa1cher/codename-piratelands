#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class MapWindow;
}

class MapWindow : public QDialog
{
    Q_OBJECT
private:
    QGraphicsScene _scene;
public:
    explicit MapWindow(QWidget *parent = nullptr);
    ~MapWindow();

private:
    Ui::MapWindow *ui;
};

#endif // MAPWINDOW_H
