#ifndef UDP_MAIN_H
#define UDP_MAIN_H

#include <QThread>
#include <QElapsedTimer>
#include <QTimer>
#include <QTime>

#include "ui_console.h"
#include "config.h"
#include "global.h"

class SDPStream {

public:
    SDPStream();

    void configure();
    void destroy();

    int source;
    QString ip_addr;
    quint16 port;
    QString protocol;
    QString video_encode;
    quint32 video_clock;

};

class UDPThread : public QThread {
    Q_OBJECT

public:
    void init(Config* ini);
    void run();
    void shutdown();

    void updateIni();

    Config* ini;

    SDPStream streams[Camera::COUNT];

public slots:
    void settingsChanged();

private:
    void print(QString text, int type);

    SystemState state;

signals:
    void updatePlayers();
    void stopPlayers();

    void consolePrint(QString text, int type);
    void logPrint(QString text, int type, QTime time);

};

#endif // UDP_MAIN_H
