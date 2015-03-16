#include "config.h"

Config::Config() {

    currSection = "";

    tcp_ipaddr = "....";
    tcp_port = 0;

    frontCam_ipaddr = "....";
    frontCam_port = 0;
    frontCam_protocol = "";
    frontCam_vencode = "";
    frontCam_vclock = 0;

    rearCam_ipaddr = "....";
    rearCam_port = 0;
    rearCam_protocol = "";
    rearCam_vencode = "";
    rearCam_vclock = 0;

    // add joystick defaults here (emergency if no ini file)

    loadIni();
}

void Config::loadIni() {

    QFile file(INI_FILE);
    if (file.open(QIODevice::ReadOnly)) {

        QTextStream ts(&file);

        while (!ts.atEnd()) {
            QString line = ts.readLine();
            if (line.contains('[') && line.contains(']')) {
                currSection = line.section('[', 1, 1).section(']', 0, 0);
            }
            if (line.contains('=')) {
                QString name = line.section('=', 0, 0);
                QString value = line.section('=', 1, 1);
                parseKey(name, value);
            }
        }

        file.close();
    }
    else {
        // ini file error
    }
}

void Config::parseKey(QString name, QString value) {

    if      (currSection == "Network") {

        if      (name == "TCP_IPADDR")              tcp_ipaddr = value;
        else if (name == "TCP_PORT")                tcp_port = value.toUInt();
    }

    else if (currSection == "Cameras") {

        if      (name == "FRONT_IPADDR")            frontCam_ipaddr = value;
        else if (name == "FRONT_PORT")              frontCam_port = value.toUInt();
        else if (name == "FRONT_PROTOCOL")          frontCam_protocol = value;
        else if (name == "FRONT_VENCODE")           frontCam_vencode = value;
        else if (name == "FRONT_VCLOCK")            frontCam_vclock = value.toUInt();

        else if (name == "REAR_IPADDR")             rearCam_ipaddr = value;
        else if (name == "REAR_PORT")               rearCam_port = value.toUInt();
        else if (name == "REAR_PROTOCOL")           rearCam_protocol = value;
        else if (name == "REAR_VENCODE")            rearCam_vencode = value;
        else if (name == "REAR_VCLOCK")             rearCam_vclock = value.toUInt();
    }

    else if (currSection == "Joystick-General") {

        if      (name == "MODIFIER_ENABLE")         modifierEnable = value.toInt();
        else if (name == "MODIFIER_BUTTON")         modifierButton = value.toInt();
    }

    else if (currSection == "Joystick-Axis") {

        for (int i = 0; i < AxisAction::COUNT; i++)
        if      (name == gs.AxisAction_Names[i]) {  axisBindings[i].control = value.section(',', 0, 0).toInt();
                                                    axisBindings[i].modifierMode = value.section(',', 1, 1).toInt(); }
    }

    else if (currSection == "Joystick-Button") {

        for (int i = 0; i < ButtonAction::COUNT; i++)
        if      (name == gs.ButtonAction_Names[i]) {buttonBindings[i].control = value.section(',', 0, 0).toInt();
                                                    buttonBindings[i].modifierMode = value.section(',', 1, 1).toInt();
                                                    buttonBindings[i].buttonMode = value.section(',', 2, 2).toInt(); }
    }
}

void Config::update() {

    QFile file(INI_FILE);
    if (file.open(QIODevice::WriteOnly)) {

        QTextStream ts(&file);

        ts << "[Network]" << "\r\n";
        ts << "TCP_IPADDR=" << tcp_ipaddr << "\r\n";
        ts << "TCP_PORT=" << tcp_port << "\r\n";
        ts << "\r\n";

        ts << "[Cameras]" << "\r\n";
        ts << "FRONT_IPADDR=" << frontCam_ipaddr << "\r\n";
        ts << "FRONT_PORT=" << frontCam_port << "\r\n";
        ts << "FRONT_PROTOCOL=" << frontCam_protocol << "\r\n";
        ts << "FRONT_VENCODE=" << frontCam_vencode << "\r\n";
        ts << "FRONT_VCLOCK=" << frontCam_vclock << "\r\n";

        ts << "REAR_IPADDR=" << rearCam_ipaddr << "\r\n";
        ts << "REAR_PORT=" << rearCam_port << "\r\n";
        ts << "REAR_PROTOCOL=" << rearCam_protocol << "\r\n";
        ts << "REAR_VENCODE=" << rearCam_vencode << "\r\n";
        ts << "REAR_VCLOCK=" << rearCam_vclock << "\r\n";
        ts << "\r\n";

        ts << "[Joystick-General]" << "\r\n";
        ts << "MODIFIER_ENABLE=" << modifierEnable << "\r\n";
        ts << "MODIFIER_BUTTON=" << modifierButton << "\r\n";
        ts << "\r\n";

        ts << "[Joystick-Axis]" << "\r\n";
        for (int i = 0; i < AxisAction::COUNT; i++)
            ts << gs.AxisAction_Names[i] << "=" << axisBindings[i].control << "," << axisBindings[i].modifierMode << "\r\n";
        ts << "\r\n";

        ts << "[Joystick-Button]" << "\r\n";
        for (int i = 0; i < ButtonAction::COUNT; i++)
            ts << gs.ButtonAction_Names[i] << "=" << buttonBindings[i].control << "," << buttonBindings[i].modifierMode << "," << buttonBindings[i].buttonMode << "\r\n";

        file.close();
    }
    else {
        // file access error
    }
}

