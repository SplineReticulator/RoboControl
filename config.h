#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QTextStream>

#include "global.h"

class Config {

public:
    Config();

    void update();

    QString tcp_ipaddr;
    quint16 tcp_port;

    QString frontCam_ipaddr;
    quint16 frontCam_port;
    QString frontCam_protocol;
    QString frontCam_vencode;
    quint32 frontCam_vclock;

    QString rearCam_ipaddr;
    quint16 rearCam_port;
    QString rearCam_protocol;
    QString rearCam_vencode;
    quint32 rearCam_vclock;

    int modifierEnable;
    int modifierButton;
    AxisBinding axisBindings[AxisAction::COUNT];
    ButtonBinding buttonBindings[ButtonAction::COUNT];

private:
    void loadIni();
    void parseKey(QString name, QString value);

    QString currSection;

    GlobalStrings gs;
};

#endif // CONFIG_H
