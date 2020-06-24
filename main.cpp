#include "mainwindow.h"

#include <QApplication>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto playerName = QInputDialog::getText(nullptr, "PirateLands", "Enter your name");
    std::shared_ptr<Game> game = std::make_shared<Game>(playerName);
    ShopWindow s(nullptr, game);
    BattleWindow bw(nullptr, game);
    MainWindow w(nullptr, &s, game);
    s.setFixedSize(s.size());
    bw.setFixedSize(bw.size());
    w.setFixedSize(w.size());
    w.show();
    return a.exec();
}
