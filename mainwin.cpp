#include "mainwin.h"

MainWindow::MainWindow(Config* ini, IOPThread* iopt, SDLThread* sdlt, TCPThread* tcpt, UDPThread* udpt) {

    this->ini = ini;
    this->iopt = iopt;
    this->sdlt = sdlt;
    this->tcpt = tcpt;
    this->udpt = udpt;

    layout = new QBoxLayout(QBoxLayout::LeftToRight);
    widget = new QWidget();

    createMenus();

    leftLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    rightLayout = new QBoxLayout(QBoxLayout::TopToBottom);

    headerLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    indicatorLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    consoleLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    columnALayout = new QBoxLayout(QBoxLayout::TopToBottom);
    columnBLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    columnCLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    columnDLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    columnELayout = new QBoxLayout(QBoxLayout::TopToBottom);

    connector = new Connector();
    console = new Console("Console");
    indicatorPanel = new IndicatorPanel();
    videoPlayer1 = new VideoPlayer("Front Camera", Camera::FRONT);
    videoPlayer2 = new VideoPlayer("Rear Camera", Camera::REAR);
    visual = new Visual3D("Visualization");

    columnALayout->addWidget(indicatorPanel->joystickGroup);
    columnALayout->addWidget(indicatorPanel->powerGroup);
    columnALayout->addStretch(1);

    columnBLayout->addWidget(indicatorPanel->driveMotorsGroup);
    columnBLayout->addWidget(indicatorPanel->flippersGroup);
    columnBLayout->addStretch(1);

    columnCLayout->addWidget(indicatorPanel->orientationGroup);
    columnCLayout->addWidget(indicatorPanel->manipulatorGroup);
    columnCLayout->addStretch(1);

    headerLayout->addLayout(indicatorPanel->infoBarLayout);
    headerLayout->addLayout(connector->layout, Qt::AlignRight);

    indicatorLayout->addLayout(columnALayout);
    indicatorLayout->addLayout(columnBLayout);
    indicatorLayout->addLayout(columnCLayout);
    indicatorLayout->addLayout(columnDLayout);
    indicatorLayout->addLayout(columnELayout);
    indicatorLayout->setStretch(0, 1);
    indicatorLayout->setStretch(1, 1);
    indicatorLayout->setStretch(2, 1);
    indicatorLayout->setStretch(3, 1);
    indicatorLayout->setStretch(4, 1);

    consoleLayout->addWidget(console->group);
    consoleLayout->addWidget(visual->group);

    leftLayout->addLayout(headerLayout);
    leftLayout->addLayout(indicatorLayout);
    leftLayout->addLayout(consoleLayout);
    leftLayout->setStretch(1, 1);
    leftLayout->setStretch(2, 1);

    rightLayout->addWidget(videoPlayer1->group);
    rightLayout->addWidget(videoPlayer2->group);
    rightLayout->setStretch(0, 1);
    rightLayout->setStretch(1, 1);

    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);

    setCentralWidget(widget);
    widget->setLayout(layout);

    setWindowTitle(INIT_WINDOW_TITLE);

    getConsoleFocus();
}

void MainWindow::connectSignals() {

    connect(connector, SIGNAL(tryConnect()), tcpt, SLOT(tryConnect()));
    connect(connector, SIGNAL(tryDisconnect()), tcpt, SLOT(tryDisconnect()));
    connect(tcpt, SIGNAL(tryConnectResult(int)), connector, SLOT(tryConnectResult(int)));
    connect(tcpt, SIGNAL(tryDisconnectResult(int)), connector, SLOT(tryDisconnectResult(int)));
    connect(connector, SIGNAL(getConsoleFocus()), this, SLOT(getConsoleFocus()));

    connect(iopt, SIGNAL(updateIndicatorPanel(Indicator*)), indicatorPanel, SLOT(update(Indicator*)));
    connect(iopt, SIGNAL(updateVisual3D(Indicator*)), visual, SLOT(update(Indicator*)));
    connect(iopt, SIGNAL(updateVideoPlayer1()), videoPlayer1, SLOT(update()));
    connect(iopt, SIGNAL(updateVideoPlayer2()), videoPlayer2, SLOT(update()));

    connect(udpt, SIGNAL(updatePlayers()), videoPlayer1, SLOT(update()));
    connect(udpt, SIGNAL(updatePlayers()), videoPlayer2, SLOT(update()));
    connect(udpt, SIGNAL(stopPlayers()), videoPlayer1, SLOT(stop()));
    connect(udpt, SIGNAL(stopPlayers()), videoPlayer2, SLOT(stop()));

    //connect(sdlt, SIGNAL(setJoystickMenuActionEnabled(bool)), joystickAction, SLOT(setEnabled(bool)));

    connect(iopt, SIGNAL(consolePrint(QString, int)), console, SLOT(print(QString, int)));
    connect(sdlt, SIGNAL(consolePrint(QString, int)), console, SLOT(print(QString, int)));
    connect(tcpt, SIGNAL(consolePrint(QString, int)), console, SLOT(print(QString, int)));
    connect(udpt, SIGNAL(consolePrint(QString, int)), console, SLOT(print(QString, int)));

    connect(console, SIGNAL(command_send()), iopt, SLOT(sendTimerExpired()));
    connect(console, SIGNAL(command_send(QString)), tcpt, SLOT(trySendText(QString)));
    connect(console, SIGNAL(command_jaxis(int, qint16)), iopt, SLOT(axisPosition(int, qint16)));
    connect(console, SIGNAL(command_jhat(int, quint8)), iopt, SLOT(hatState(int, quint8)));
    connect(console, SIGNAL(command_jbutton(int, quint8)), iopt, SLOT(buttonState(int, quint8)));
    connect(console, SIGNAL(command_resetcam1()), videoPlayer1, SLOT(update()));
    connect(console, SIGNAL(command_resetcam2()), videoPlayer2, SLOT(update()));
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {

    if (event->type() == QEvent::KeyPress) {

        Q_UNUSED(watched);
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        switch (keyEvent->key()) {

        case Qt::Key_F10:
            connector->button->click();
            return true;

        default:
            return false;
        }
    }
    else {
        return false;
    }
}

void MainWindow::createMenus() {

    fileMenu = menuBar()->addMenu("File");

    viewMenu = menuBar()->addMenu("View");
    fullScreenAction = new QAction("Full Screen", this);
    fullScreenAction->setShortcut(Qt::Key_F11);
    fullScreenAction->setCheckable(true);
    viewMenu->addAction(fullScreenAction);

    settingsMenu = menuBar()->addMenu("Settings");
    networkAction = new QAction("Network...", this);
    networkAction->setShortcut(Qt::CTRL + Qt::Key_N);
    joystickAction = new QAction("Joystick...", this);
    joystickAction->setShortcut(Qt::CTRL + Qt::Key_J);
    cameraAction = new QAction("Cameras...", this);
    cameraAction->setShortcut(Qt::CTRL + Qt::Key_M);
    settingsMenu->addAction(networkAction);
    settingsMenu->addAction(joystickAction);
    settingsMenu->addAction(cameraAction);

    helpMenu = menuBar()->addMenu("Help");

    connect(fullScreenAction, SIGNAL(triggered(bool)), this, SLOT(toggleFullScreen(bool)));
    connect(networkAction, SIGNAL(triggered()), this, SLOT(showNetworkSettings()));
    connect(joystickAction, SIGNAL(triggered()), this, SLOT(showJoystickSettings()));
    connect(cameraAction, SIGNAL(triggered()), this, SLOT(showCameraSettings()));
}

void MainWindow::resolutionChanged(QRect& geometry) {

    if (geometry.width() == 1920 && geometry.height() == 1080) {
        layout->setStretch(0, 93);
        layout->setStretch(1, 48);
        consoleLayout->setStretch(0, 82);
        consoleLayout->setStretch(1, 48);
    }
    else if (geometry.width() == 1366 && geometry.height() == 768) {
        layout->setStretch(0, 100);
        layout->setStretch(1, 48);
        consoleLayout->setStretch(0, 90);
        consoleLayout->setStretch(1, 48);
    }
    else {
        layout->setStretch(0, 2);
        layout->setStretch(1, 1);
        consoleLayout->setStretch(0, 2);
        consoleLayout->setStretch(1, 1);
    }

    move(geometry.width()/2-width()/2, geometry.height()/2-height()/2);
}

void MainWindow::getConsoleFocus() {

    console->input->setFocus();
}

void MainWindow::toggleFullScreen(bool full) {

    if (full) showFullScreen();
    else showNormal();
}

void MainWindow::showNetworkSettings() {

    NetworkDialog dialog(tcpt);
    connect(&dialog, SIGNAL(settingsChanged()), tcpt, SLOT(settingsChanged()));
    dialog.exec();
}

void MainWindow::showJoystickSettings() {

    JoystickDialog dialog(iopt, sdlt);
    connect(&dialog, SIGNAL(settingsChanged()), iopt, SLOT(settingsChanged()));
    dialog.exec();
}

void MainWindow::showCameraSettings() {

    CameraDialog dialog(udpt);
    connect(&dialog, SIGNAL(settingsChanged()), udpt, SLOT(settingsChanged()));
    dialog.exec();
}

