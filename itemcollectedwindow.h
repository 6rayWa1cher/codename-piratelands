#ifndef ITEMCOLLECTEDWINDOW_H
#define ITEMCOLLECTEDWINDOW_H

#include <QDialog>

namespace Ui {
class itemcollectedwindow;
}

class itemcollectedwindow : public QDialog
{
    Q_OBJECT

public:
    explicit itemcollectedwindow(QWidget *parent = nullptr);
    ~itemcollectedwindow();

private:
    Ui::itemcollectedwindow *ui;
};

#endif // ITEMCOLLECTEDWINDOW_H
