#include "mainwindow.h"
#include "homewindow.h"
#include "Peer.h"
#include "user.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Peer *peer= new Peer(stoi(argv[1]));
    User *user = new User(peer);
    peer->setDS("10.7.57.207", 88);
    QApplication a(argc, argv);
    MainWindow w(user);
    w.show();
    return a.exec();
}
