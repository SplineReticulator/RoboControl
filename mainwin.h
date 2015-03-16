#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <QScreen>

#include <QAction>
#include <QGridLayout>
#include <QBoxLayout>
#include <QMenuBar>
#include <QWidget>
#include <QTimer>

#include "IOPmain.h"
#include "SDLmain.h"
#include "TCPmain.h"
#include "UDPmain.h"
#include "config.h"
#include "global.h"

#include "ui_connector.h"
#include "ui_console.h"
#include "ui_indicatorpanel.h"
#include "ui_videoplayer.h"
#include "ui_visual3d.h"

#include "dialog_network.h"
#include "dialog_joystick.h"
#include "dialog_camera.h"

#define INIT_WINDOW_TITLE "RoboControl"
#define INIT_WINDOW_WIDTH 1366
#define INIT_WINDOW_HEIGHT 768

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Config* ini, IOPThread* iopt, SDLThread* sdlt, TCPThread* tcpt, UDPThread* udpt);

    bool eventFilter(QObject* watched, QEvent* event);

    void connectSignals();

public slots:
    void resolutionChanged(QRect& geometry);
    void getConsoleFocus();
    void toggleFullScreen(bool full);
    void showNetworkSettings();
    void showCameraSettings();
    void showJoystickSettings();

private:
    void createMenus();

    Config* ini;

    IOPThread* iopt;
    SDLThread* sdlt;
    TCPThread* tcpt;
    UDPThread* udpt;

    // LAYOUT
    QWidget* widget;
    QBoxLayout* layout;

    QBoxLayout* leftLayout;
    QBoxLayout* rightLayout;

    QBoxLayout* headerLayout;
    QBoxLayout* indicatorLayout;
    QBoxLayout* consoleLayout;

    QBoxLayout* columnALayout;
    QBoxLayout* columnBLayout;
    QBoxLayout* columnCLayout;
    QBoxLayout* columnDLayout;
    QBoxLayout* columnELayout;

    // MENU
    QMenu* fileMenu;

    QMenu* viewMenu;
    QAction* fullScreenAction;

    QMenu* settingsMenu;
    QAction* networkAction;
    QAction* joystickAction;
    QAction* cameraAction;

    QMenu* helpMenu;

    // WIDGETS
    Connector* connector;
    Console* console;
    IndicatorPanel* indicatorPanel;
    VideoPlayer* videoPlayer1;
    VideoPlayer* videoPlayer2;
    Visual3D* visual;
};

#endif // MAINWIN_H
