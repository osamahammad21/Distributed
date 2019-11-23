QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    homewindow.cpp \
    imagesamplewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    photolistwidget.cpp \
    photosettingswindow.cpp \
    photoswidget.cpp \
    registerwindow.cpp \
    uploadphotowindow.cpp \
    user.cpp \
    userslist.cpp \
    userswidget.cpp \
    viewimagesamples.cpp \
    viewphotowindow.cpp \
    base64.cpp \
    Message.cpp \
    Image.cpp \
    UDPSocket.cpp \
    Peer.cpp

HEADERS += \
    ImageModel.h \
    homewindow.h \
    imagesamplewidget.h \
    mainwindow.h \
    photolistwidget.h \
    photosettingswindow.h \
    photoswidget.h \
    registerwindow.h \
    uploadphotowindow.h \
    user.h \
    userslist.h \
    userswidget.h \
    viewimagesamples.h \
    viewphotowindow.h \
    base64.h \
    Message.h \
    Image.h \
    UDPSocket.h \
    Peer.h

FORMS += \
    homewindow.ui \
    imagesamplewidget.ui \
    mainwindow.ui \
    photolistwidget.ui \
    photosettingswindow.ui \
    photoswidget.ui \
    registerwindow.ui \
    uploadphotowindow.ui \
    userslist.ui \
    userswidget.ui \
    viewimagesamples.ui \
    viewphotowindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target