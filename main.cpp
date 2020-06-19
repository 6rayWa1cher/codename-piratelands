#include "mainwindow.h"

#include <QApplication>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto playerName = QInputDialog::getText(nullptr, "PirateLands", "Enter your name");
    MainWindow w(nullptr, std::make_shared<Game>(playerName));
    w.show();
    return a.exec();
}
