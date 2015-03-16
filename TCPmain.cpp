#include "TCPmain.h"

void TCPThread::init(Config* ini) {

    systemState = SS_INIT;

    this->ini = ini;

    ipaddr = ini->tcp_ipaddr;
    port = ini->tcp_port;

    connectionState = CS_DISCONNECTED;
    writeState = WS_IDLE;
}

void TCPThread::run() {

    if (systemState != SS_CLOSING) systemState = SS_RUNNING;

    socket = new QTcpSocket();\

    while (systemState == SS_RUNNING) {

        switch (connectionState) {

        case CS_CONNECTING:

            socket->connectToHost(ipaddr, port);
            if (socket->waitForConnected(CONN_TIMEOUT_MS)) {
                connectionState = CS_CONNECTED;
                emit tryConnectResult(1);
                print(QString("Connected to host %1 on port %2.").arg(ipaddr).arg(port), PT_SUCCESS);
            }
            else {
                connectionState = CS_DISCONNECTED;
                emit tryConnectResult(0);
                print(QString("Could not connect to host %1 on port %2.").arg(ipaddr).arg(port), PT_ERROR);
            }

            break;

        case CS_DISCONNECTING:

            socket->disconnectFromHost();
            if (socket->state() == QAbstractSocket::UnconnectedState || socket->waitForDisconnected(CONN_TIMEOUT_MS)) {
                connectionState = CS_DISCONNECTED;
                emit tryDisconnectResult(1);
                print(QString("Disconnected from host %1.").arg(ipaddr), PT_WARNING);
            }
            else {
                connectionState = CS_CONNECTED;
                emit tryDisconnectResult(0);
                print(QString("Could not disconnect from host %1.").arg(ipaddr), PT_ERROR);
            }

            break;

        case CS_CONNECTED:

            if (socket->state() == QAbstractSocket::ConnectedState) {

                if (writeState == WS_READY) {
                    writeState = WS_BUSY;
                    socket->write(wbuffer);
                    if (socket->waitForBytesWritten(WAIT_BYTESWRITTEN_MS)) {
                        emit trySendResult(1);
                    }
                    else {
                        emit trySendResult(0);
                    }
                    writeState = WS_IDLE;
                }

                if (socket->waitForReadyRead(WAIT_READYREAD_MS) && socket->bytesAvailable() > 0) {
                    // Next two lines are a hack...a library bug causes crash if all bytes are read at once (only in release build)
                    rbuffer = socket->read(socket->bytesAvailable()-1);
                    rbuffer.append(socket->read(1));
                    emit dataReceived(rbuffer);
                }
            }
            else {
                connectionState = CS_DISCONNECTING;
            }

            break;

        case CS_DISCONNECTED:

            usleep(TCPT_USLEEP);

            break;
        }

    }

}

void TCPThread::shutdown() {

    updateIni();

    connectionState = CS_DISCONNECTING;
    while (connectionState != CS_DISCONNECTED)
        msleep(100);

    systemState = SS_CLOSING;
}

void TCPThread::updateIni() {

    ini->tcp_ipaddr = ipaddr;
    ini->tcp_port = port;
}

void TCPThread::tryConnect() {

    if (connectionState == CS_DISCONNECTED) connectionState = CS_CONNECTING;
}

void TCPThread::tryDisconnect() {

    if (connectionState == CS_CONNECTED) connectionState = CS_DISCONNECTING;
}

void TCPThread::trySend(QByteArray data) {

    wbuffer = data;
    if (connectionState == CS_CONNECTED && writeState == WS_IDLE) writeState = WS_READY;
}

void TCPThread::trySendText(QString data) {

    wbuffer = data.toUtf8();
    if (connectionState == CS_CONNECTED && writeState == WS_IDLE) writeState = WS_READY;
}

void TCPThread::settingsChanged() {

    tryDisconnect();
}

void TCPThread::print(QString text, int type) {

    QTime time = QTime::currentTime();
    emit consolePrint(text, type);
    emit logPrint(text, type, time);
}

