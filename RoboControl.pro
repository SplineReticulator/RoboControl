#-------------------------------------------------
#
# Project created by QtCreator 2014-11-25T16:42:27
#
#-------------------------------------------------

QT += core gui network opengl widgets multimedia multimediawidgets

TARGET = RoboControl
TEMPLATE = app
CONFIG -= windows
QMAKE_LFLAGS += $$QMAKE_LFLAGS_WINDOWS

INCLUDEPATH += "$$PWD/SDL2/include" \ "$$PWD/VLC/include"

SOURCES += main.cpp \
    SDLmain.cpp \
    TCPmain.cpp \
    IOPmain.cpp \
    ui_console.cpp \
    config.cpp \
    dialog_joystick.cpp \
    global.cpp \
    ui_indicatorpanel.cpp \
    ui_videoplayer.cpp \
    UDPmain.cpp \
    dialog_network.cpp \
    log.cpp \
    ui_iconindicator.cpp \
    ui_visual3d.cpp \
    mainwin.cpp \
    dialog_camera.cpp \
    ui_connector.cpp

LIBS += -L"$$PWD/SDL2/lib" -lSDL2 -lSDL2main
LIBS += -L"$$PWD/VLC/lib" -lvlc-qt -lvlc-qt-widgets

QMAKE_CXXFLAGS += /MD

HEADERS += \
    main.h \
    SDLmain.h \
    TCPmain.h \
    IOPmain.h \
    ui_console.h \
    config.h \
    dialog_joystick.h \
    global.h \
    ui_indicatorpanel.h \
    ui_videoplayer.h \
    UDPmain.h \
    dialog_network.h \
    log.h \
    ui_iconindicator.h \
    ui_visual3d.h \
    mainwin.h \
    dialog_camera.h \
    ui_connector.h

RESOURCES += resources.qrc

RC_FILE += icon.rc
