#ifndef BATTLELOSTWINDOW_H
#define BATTLELOSTWINDOW_H

#include <QDialog>

namespace Ui {
class battlelostwindow;
}

class battlelostwindow : public QDialog
{
    Q_OBJECT

public:
    explicit battlelostwindow(QWidget *parent = nullptr);
    ~battlelostwindow();

private:
    Ui::battlelostwindow *ui;

public slots:
    void _continue();
};

#endif // BATTLELOSTWINDOW_H
