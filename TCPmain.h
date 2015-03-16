#ifndef TCP_MAIN_H
#define TCP_MAIN_H

#include <QThread>
#include <QElapsedTimer>
#include <QTime>
#include <QTcpSocket>

#include "ui_console.h"
#include "config.h"
#include "global.h"

class TCPThread : public QThread {
    Q_OBJECT

public:
    void init(Config* ini);
    void run();
    void shutdown();

    void updateIni();

    Config* ini;

    QString ipaddr;
    quint16 port;

public slots:
    void tryConnect();
    void tryDisconnect();
    void trySend(QByteArray data);
    void trySendText(QString data);

    void settingsChanged();

private:
    void print(QString text, int type);

    SystemState systemState;
    ConnectionState connectionState;
    WriteState writeState;

    QTcpSocket* socket;
    QByteArray wbuffer;
    QByteArray rbuffer;

signals:
    void tryConnectResult(int success);
    void tryDisconnectResult(int success);
    void trySendResult(int success);
    void dataReceived(QByteArray data);

    void consolePrint(QString text, int type);
    void logPrint(QString text, int type, QTime time);

};

#endif // TCP_MAIN_H
