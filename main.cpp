#include "mainwindow.h"
#include "homewindow.h"
#include "Peer.h"
#include "user.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Peer *peer= new Peer(74);
    User *user = new User(peer);
    peer->setDS("10.0.2.15", 88);
    QApplication a(argc, argv);
    MainWindow w(user);
    w.show();
    return a.exec();
}
