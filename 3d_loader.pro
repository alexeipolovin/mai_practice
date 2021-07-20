QT       += core gui 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation opengl websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets websockets

CONFIG += c++11
LIBS += -lOpengl32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(libraries/qmarkdowntextedit/qmarkdowntextedit.pri)

SOURCES += \
    m3dpreview.cpp \
    main.cpp \
    oglwidg.cpp \
    mainwindow.cpp \
    settingswindow.cpp \
    appearancesettings.cpp \
    socketclient.cpp \
    untextedit.cpp \
    uncomplete.cpp \
    webconnector.cpp \


HEADERS += \
    m3dpreview.h \
    modelloader.h \
    oglwidg.h \
    mainwindow.h \
    settingswindow.h \
    appearancesettings.h \
    socketclient.h \
    untextedit.h \
    uncomplete.h \
    webconnector.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    shaders.qrc
