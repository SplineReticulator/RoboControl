#ifndef IOP_MAIN_H
#define IOP_MAIN_H

#include <QThread>
#include <QElapsedTimer>
#include <QTimer>
#include <QTime>
#include <QByteArray>

#include "ui_console.h"
#include "config.h"
#include "global.h"

class IOPThread : public QThread {
    Q_OBJECT

public:
    void init(Config* ini);
    void run();
    void shutdown();

    void initJoystickBindings();
    void initJoystickReadings();
    void initJoystickActions();
    void initControlOut();

    void initDeviceProperties();
    void initIndicatorValues();
    void initFeedbackIn();

    void updateIni();

    Config* ini;

    ControlOut* co;
    FeedbackIn* fi;

    QByteArray co_bytes;
    QByteArray fi_bytes;

    bool sendControlOut_flag;
    bool parseFeedbackIn_flag;

    // ======== ControlOut ======== //

    int modifierEnable;
    int modifierButton;
    AxisBinding axisBindings[AxisAction::COUNT];
    ButtonBinding buttonBindings[ButtonAction::COUNT];

    quint16 axis[AxisAction::COUNT];
    quint8 button[ButtonAction::COUNT];

    quint16 jaxis[AxisControl::COUNT];
    quint8 jpersist[ButtonControl::COUNT];
    quint8 jtoggle[ButtonControl::COUNT];

    // ======== FeedbackIn ======== //

    Device dev[Device::COUNT];
    Indicator ind[Indicator::COUNT];

public slots:
    void axisPosition(int axis, qint16 position);
    void hatState(int hat, quint8 state);
    void buttonState(int button, quint8 state);

    void sendTimerExpired();
    void trySendResult(int success);
    void dataReceived(QByteArray data);

    void settingsChanged();

private:
    void updateActions();
    void updateControlOut();
    void sendControlOut();

    void parseFeedbackIn();
    void updateIndicatorf(int indicator, int device, int data_index, qreal coeff, int decimal_places, QString units);
    void updateIndicatorValuef(int indicator, qreal value, quint8 status, int decimal_places, QString units);
    void updateIndicatori(int indicator, int device, int data_index, QString units);
    void updateIndicatorValuei(int indicator, int value, quint8 status, QString units);

    void print(QString text, int type);

    SystemState state;

signals:
    void trySend(QByteArray data);
    void updateIndicatorPanel(Indicator* ind);
    void updateVisual3D(Indicator* ind);
    void updateVideoPlayer1();
    void updateVideoPlayer2();

    void consolePrint(QString text, int type);
    void logPrint(QString text, int type, QTime time);

};

#endif // IOP_MAIN_H
