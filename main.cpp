#include "mainwindow.h"
#include "homewindow.h"
#include "Peer.h"
#include "user.h"
#include <QApplication>
#include <pthread.h>

using namespace std;
int main(int argc, char *argv[])
{
    Peer *peer= new Peer(stoi(argv[1]));
    User *user = new User(peer);
    peer->setDS("directoryGroupH", 89);
    QApplication a(argc, argv);
    MainWindow w(user);
    w.show();
    return a.exec();
}

//requestAccess does not update the local copy
