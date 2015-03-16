#include "main.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QScreen* screen = app.primaryScreen();

    //QPixmap splash_image(":/images/splash.png");
    //QSplashScreen splash(splash_image);
    //splash.show();

    Config ini;
    Log log;

    IOPThread iopt;
    SDLThread sdlt;
    TCPThread tcpt;
    UDPThread udpt;

    MainWindow win(&ini, &iopt, &sdlt, &tcpt, &udpt);

    // CONNECT SIGNALS
    QObject::connect(screen, SIGNAL(geometryChanged(QRect*)), &win, SLOT(resolutionChanged(QRect*)));

    QObject::connect(&iopt, SIGNAL(trySend(QByteArray)), &tcpt, SLOT(trySend(QByteArray)));
    QObject::connect(&tcpt, SIGNAL(trySendResult(int)), &iopt, SLOT(trySendResult(int)));
    QObject::connect(&tcpt, SIGNAL(dataReceived(QByteArray)), &iopt, SLOT(dataReceived(QByteArray)));

    QObject::connect(&sdlt, SIGNAL(axisPosition(int, qint16)), &iopt, SLOT(axisPosition(int, qint16)));
    QObject::connect(&sdlt, SIGNAL(hatState(int, quint8)), &iopt, SLOT(hatState(int, quint8)));
    QObject::connect(&sdlt, SIGNAL(buttonState(int, quint8)), &iopt, SLOT(buttonState(int, quint8)));

    QObject::connect(&iopt, SIGNAL(logPrint(QString, int, QTime)), &log, SLOT(print(QString, int, QTime)));
    QObject::connect(&sdlt, SIGNAL(logPrint(QString, int, QTime)), &log, SLOT(print(QString, int, QTime)));
    QObject::connect(&tcpt, SIGNAL(logPrint(QString, int, QTime)), &log, SLOT(print(QString, int, QTime)));
    QObject::connect(&udpt, SIGNAL(logPrint(QString, int, QTime)), &log, SLOT(print(QString, int, QTime)));

    QObject::connect(&iopt, SIGNAL(finished()), &app, SLOT(quit()));
    QObject::connect(&sdlt, SIGNAL(finished()), &app, SLOT(quit()));
    QObject::connect(&tcpt, SIGNAL(finished()), &app, SLOT(quit()));
    QObject::connect(&udpt, SIGNAL(finished()), &app, SLOT(quit()));

    // TIMERS
    QTimer controlOutTimer;
    controlOutTimer.start(CONTROLOUT_TIMER_MS);
    QObject::connect(&controlOutTimer, SIGNAL(timeout()), &iopt, SLOT(sendTimerExpired()));

    win.connectSignals();
    win.installEventFilter(&win);

    iopt.init(&ini);
    sdlt.init(&ini);
    tcpt.init(&ini);
    udpt.init(&ini);

    iopt.start();
    sdlt.start();
    tcpt.start();
    udpt.start();

    win.show();
    win.resolutionChanged(screen->geometry());
    //splash.finish(&win);
    app.exec();

    udpt.shutdown();
    tcpt.shutdown();
    sdlt.shutdown();
    iopt.shutdown();

    udpt.wait();
    tcpt.wait();
    sdlt.wait();
    iopt.wait();

    log.close();
    ini.update();

    return 0;
}
