#ifndef COPYRIGHTWINDOW_H
#define COPYRIGHTWINDOW_H

#include <QDialog>

namespace Ui {
class CopyrightWindow;
}

class CopyrightWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CopyrightWindow(QWidget *parent = nullptr);
    ~CopyrightWindow();

private:
    Ui::CopyrightWindow *ui;
};

#endif // COPYRIGHTWINDOW_H
