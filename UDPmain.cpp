#include "UDPmain.h"

SDPStream::SDPStream() {

    source = -1;
    ip_addr = "";
    port = 0;
    protocol = "";
    video_encode = "";
    video_clock = 0;
}

void SDPStream::configure() {

    QFile file(QString("stream%1.sdp").arg(source));
    if (file.open(QIODevice::WriteOnly)) {

        QTextStream ts(&file);

        ts << "v=0" << "\r\n";
        ts << "c=IN IP4 " << ip_addr << "\r\n";
        ts << "m=video " << port << " " << protocol << " 96" << "\r\n";
        ts << "a=rtpmap:96 " << video_encode << "/" << video_clock << "\r\n";
        ts << "t=0 0";

        file.close();
    }
    else {
        // sdp file error
    }
}

void SDPStream::destroy() {

    QFile file(QString("stream%1.sdp").arg(source));
    file.remove();
}

void UDPThread::init(Config* ini) {

    state = SS_INIT;

    this->ini = ini;

    for (int i = 0; i < Camera::COUNT; i++) {
        streams[i].source = i;
    }

    streams[Camera::FRONT].ip_addr = ini->frontCam_ipaddr;
    streams[Camera::FRONT].port = ini->frontCam_port;
    streams[Camera::FRONT].protocol = ini->frontCam_protocol;
    streams[Camera::FRONT].video_encode = ini->frontCam_vencode;
    streams[Camera::FRONT].video_clock = ini->frontCam_vclock;

    streams[Camera::REAR].ip_addr = ini->rearCam_ipaddr;
    streams[Camera::REAR].port = ini->rearCam_port;
    streams[Camera::REAR].protocol = ini->rearCam_protocol;
    streams[Camera::REAR].video_encode = ini->rearCam_vencode;
    streams[Camera::REAR].video_clock = ini->rearCam_vclock;

    settingsChanged();
}

void UDPThread::run() {

    if (state != SS_CLOSING) state = SS_RUNNING;

    while (state == SS_RUNNING) {

        usleep(UDPT_USLEEP);
    }
}

void UDPThread::shutdown() {

    emit stopPlayers();
    for (int i = 0; i < Camera::COUNT; i++) {
        streams[i].destroy();
    }

    updateIni();

    state = SS_CLOSING;
}

void UDPThread::updateIni() {

    ini->frontCam_ipaddr = streams[Camera::FRONT].ip_addr;
    ini->frontCam_port = streams[Camera::FRONT].port;
    ini->frontCam_protocol = streams[Camera::FRONT].protocol;
    ini->frontCam_vencode = streams[Camera::FRONT].video_encode;
    ini->frontCam_vclock = streams[Camera::FRONT].video_clock;

    ini->rearCam_ipaddr = streams[Camera::REAR].ip_addr;
    ini->rearCam_port = streams[Camera::REAR].port;
    ini->rearCam_protocol = streams[Camera::REAR].protocol;
    ini->rearCam_vencode = streams[Camera::REAR].video_encode;
    ini->rearCam_vclock = streams[Camera::REAR].video_clock;
}

void UDPThread::settingsChanged() {

    for (int i = 0; i < Camera::COUNT; i++) {
        streams[i].configure();
    }
    emit updatePlayers();
}

void UDPThread::print(QString text, int type) {

    QTime time = QTime::currentTime();
    emit consolePrint(text, type);
    emit logPrint(text, type, time);
}
