#include "mainwindow.h"
#include "homewindow.h"
#include "Peer.h"
#include "user.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Peer *peer= new Peer(73);
    User *user = new User(peer);
    peer->setDS("127.0.0.1", 88);
    QApplication a(argc, argv);
    MainWindow w(user);
    w.show();
    return a.exec();
}
