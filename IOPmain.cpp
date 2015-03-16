#include "IOPmain.h"

void IOPThread::init(Config* ini) {

    state = SS_INIT;

    this->ini = ini;

    co = new ControlOut();
    fi = new FeedbackIn();

    initJoystickBindings();
    initJoystickReadings();
    initJoystickActions();
    initControlOut();

    initDeviceProperties();
    initIndicatorValues();
    initFeedbackIn();

    sendControlOut_flag = false;
    parseFeedbackIn_flag = false;
}

void IOPThread::run() {

    if (state != SS_CLOSING) state = SS_RUNNING;



    while (state == SS_RUNNING) {

        updateActions();
        updateControlOut();

        if (sendControlOut_flag) sendControlOut();
        if (parseFeedbackIn_flag) parseFeedbackIn();

        usleep(IOPT_USLEEP);
    }
}

void IOPThread::shutdown() {

    updateIni();

    state = SS_CLOSING;
}

void IOPThread::initJoystickBindings() {

    modifierEnable = ini->modifierEnable;
    modifierButton = ini->modifierButton;

    for (int i = 0; i < AxisAction::COUNT; i++) {
        axisBindings[i].control = ini->axisBindings[i].control;
        axisBindings[i].modifierMode = ini->axisBindings[i].modifierMode;
    }

    for (int i = 0; i < ButtonAction::COUNT; i++) {
        buttonBindings[i].control = ini->buttonBindings[i].control;
        buttonBindings[i].modifierMode = ini->buttonBindings[i].modifierMode;
        buttonBindings[i].buttonMode = ini->buttonBindings[i].buttonMode;
    }
}

void IOPThread::initJoystickReadings() {

    for (int i = 0; i < AxisControl::COUNT; i++) {
        jaxis[i] = 0;
    }
    for (int i = 0; i < ButtonControl::COUNT; i++) {
        jpersist[i] = 0;
        jtoggle[i] = 0;
    }
}

void IOPThread::initJoystickActions() {

    for (int i = 0; i < AxisAction::COUNT; i++) {
        axis[i] = 0;
    }
    for (int i = 0; i < ButtonAction::COUNT; i++) {
        button[i] = 0;
    }
}

void IOPThread::initControlOut() {

    co->drive_speed = 0x00;
    co->yaw_speed = 0x00;
    co->flippers = 0x00;
    co->arm_lights = 0x00;
    co->cameras_stop = 0x00;
}

void IOPThread::initDeviceProperties() {

    for (int i = 0; i < Device::COUNT; i++) {
        dev[i].numParams = 0;
        dev[i].status = 0;
        for (int j = 0; j < MAX_DEVICE_PARAMS; j++) {
            dev[i].data[j] = 0;
        }
    }

    dev[Device::RIGHT_DRIVE_ENCODER].numParams = 1;
    dev[Device::LEFT_DRIVE_ENCODER].numParams = 1;
    dev[Device::FRONT_FLIPPER_ENCODER].numParams = 1;
    dev[Device::BACK_FLIPPER_ENCODER].numParams = 1;
    dev[Device::AGM_BNO055].numParams = 6;
    dev[Device::DRIVE_MOTOR_CONTROLLER].numParams = 2;
    dev[Device::FLIPPER_MOTOR_CONTROLLER].numParams = 2;
    dev[Device::INTERPROCESSOR_COM_HANDLER].numParams = 0;
    dev[Device::G15_SERVO_BOTTOM].numParams = 1;
    dev[Device::G15_SERVO_MIDDLE].numParams = 1;
    dev[Device::G15_SERVO_TOP].numParams = 1;
    dev[Device::G15_SERVO_PAN].numParams = 1;
    dev[Device::MANIPULATOR_ARM].numParams = 4;
    dev[Device::TEMP_SENSOR_BATTERY].numParams = 1;
    dev[Device::TEMP_SENSOR_LEFT_DRIVE_MOTOR].numParams = 1;
    dev[Device::TEMP_SENSOR_RIGHT_DRIVE_MOTOR].numParams = 1;
    dev[Device::TEMP_SENSOR_FRONT_FLIPPER_MOTOR].numParams = 1;
    dev[Device::TEMP_SENSOR_BACK_FLIPPER_MOTOR].numParams = 1;
    dev[Device::VOLT_SENSOR_BATTERY].numParams = 1;
    dev[Device::VIDEO_SERVER].numParams = 1;
}

void IOPThread::initIndicatorValues() {

    for (int i = 0; i < Indicator::COUNT; i++) {
        ind[i].i = 0;
        ind[i].f = 0;
        ind[i].str = "";
        ind[i].status = 0;
    }
}

void IOPThread::initFeedbackIn() {

    fi->device = 0x00;
    fi->status = 0x00;
    for (int i = 0; i < MAX_DEVICE_PARAMS; i++) {
        fi->data[i] = 0x0000;
    }
}

void IOPThread::updateIni() {

    ini->modifierEnable = modifierEnable;
    ini->modifierButton = modifierButton;

    for (int i = 0; i < AxisAction::COUNT; i++) {
        ini->axisBindings[i].control = axisBindings[i].control;
        ini->axisBindings[i].modifierMode = axisBindings[i].modifierMode;
    }

    for (int i = 0; i < ButtonAction::COUNT; i++) {
        ini->buttonBindings[i].control = buttonBindings[i].control;
        ini->buttonBindings[i].modifierMode = buttonBindings[i].modifierMode;
        ini->buttonBindings[i].buttonMode = buttonBindings[i].buttonMode;
    }
}

void IOPThread::updateActions() {

    for (int i = 0; i < AxisAction::COUNT; i++) {

        if (axisBindings[i].control != -1) {

            switch (axisBindings[i].modifierMode) {

            case UNMODIFIED:
                if (!modifierEnable || !jpersist[modifierButton]) {
                    axis[i] = jaxis[axisBindings[i].control];
                }
                else {
                    axis[i] = 0;
                }
                break;

            case ALWAYS:
                axis[i] = jaxis[axisBindings[i].control];
                break;

            case MODIFIED:
                if (modifierEnable && jpersist[modifierButton]) {
                    axis[i] = jaxis[axisBindings[i].control];
                }
                else {
                    axis[i] = 0;
                }
                break;

            default:
                break;
            }
        }
    }

    for (int i = 0; i < ButtonAction::COUNT; i++) {

        if (buttonBindings[i].control != -1) {

            switch (buttonBindings[i].modifierMode) {

            case UNMODIFIED:
                switch (buttonBindings[i].buttonMode) {
                case TOGGLE:
                    if ((!modifierEnable || !jpersist[modifierButton]) && jtoggle[buttonBindings[i].control]) {
                        button[i] = 1;
                    }
                    jtoggle[buttonBindings[i].control] = 0;
                    break;
                case PERSIST:
                    button[i] = (!modifierEnable || !jpersist[modifierButton]) && jpersist[buttonBindings[i].control] ? 1 : 0;
                    break;
                default:
                    break;
                }
                break;

            case ALWAYS:
                switch (buttonBindings[i].buttonMode) {
                case TOGGLE:
                    if (jtoggle[buttonBindings[i].control]) {
                        button[i] = 1;
                    }
                    jtoggle[buttonBindings[i].control] = 0;
                    break;
                case PERSIST:
                    button[i] = jpersist[buttonBindings[i].control] ? 1 : 0;
                    break;
                default:
                    break;
                }
                break;

            case MODIFIED:
                switch (buttonBindings[i].buttonMode) {
                case TOGGLE:
                    if ((!modifierEnable || jpersist[modifierButton]) && jtoggle[buttonBindings[i].control]) {
                        button[i] = 1;
                    }
                    jtoggle[buttonBindings[i].control] = 0;
                    break;
                case PERSIST:
                    button[i] = (!modifierEnable || jpersist[modifierButton]) && jpersist[buttonBindings[i].control] ? 1 : 0;
                    break;
                default:
                    break;
                }
                break;

            default:
                break;
            }
        }
    }
}

void IOPThread::updateControlOut() {

    // DRIVE SPEED
    if (axis[AxisAction::DRIVE_FORWARD]) {
        co->drive_speed = (axis[AxisAction::DRIVE_FORWARD]*axis[AxisAction::THROTTLE])/(32768*256);
    }
    else if (axis[AxisAction::DRIVE_REVERSE]) {
        co->drive_speed = 0x80 | (axis[AxisAction::DRIVE_REVERSE]*axis[AxisAction::THROTTLE])/(32768*256);
    }
    else {
        co->drive_speed = 0;
    }

    // YAW SPEED
    if (axis[AxisAction::YAW_LEFT]) {
        co->yaw_speed = (axis[AxisAction::YAW_LEFT]*axis[AxisAction::THROTTLE])/(32768*256);
    }
    else if (axis[AxisAction::YAW_RIGHT]) {
        co->yaw_speed = 0x80 | (axis[AxisAction::YAW_RIGHT]*axis[AxisAction::THROTTLE])/(32768*256);
    }
    else {
        co->yaw_speed = 0;
    }

    // FLIPPERS
    co->flippers = 0;
    co->flippers |= button[ButtonAction::FRONT_FLIP_REVERSE] << 7;
    co->flippers |= button[ButtonAction::FRONT_FLIP_FORWARD] << 6;
    co->flippers |= button[ButtonAction::BACK_FLIP_REVERSE] << 5;
    co->flippers |= button[ButtonAction::BACK_FLIP_FORWARD] << 4;

    // MANIPULATOR ARM / LIGHTS
    co->arm_lights = 0;
    co->arm_lights |= button[ButtonAction::ARM_LOWER] << 7;
    co->arm_lights |= button[ButtonAction::ARM_RAISE] << 6;
    co->arm_lights |= button[ButtonAction::PAN_RIGHT] << 5;
    co->arm_lights |= button[ButtonAction::PAN_LEFT] << 4;
    co->arm_lights |= button[ButtonAction::TILT_DOWN] << 3;
    co->arm_lights |= button[ButtonAction::TILT_UP] << 2;
    co->arm_lights |= button[ButtonAction::LIGHTS_ON_OFF] << 1;
    co->arm_lights |= button[ButtonAction::LIGHTS_ON_OFF] << 0;

    // CAMERAS / STOP COMMANDS
    co->cameras_stop = 0;
    co->cameras_stop |= button[ButtonAction::EMERGENCY_STOP] << 7;
    co->cameras_stop |= button[ButtonAction::EMERGENCY_STOP] << 6;
    co->cameras_stop |= button[ButtonAction::SHUTDOWN] << 5;
    co->cameras_stop |= button[ButtonAction::SHUTDOWN] << 4;
    co->cameras_stop |= button[ButtonAction::FRONT_CAM_POWER] << 3;
    co->cameras_stop |= button[ButtonAction::FRONT_CAM_POWER] << 2;
    co->cameras_stop |= button[ButtonAction::REAR_CAM_POWER] << 1;
    co->cameras_stop |= button[ButtonAction::REAR_CAM_POWER] << 0;
}

void IOPThread::sendControlOut() {

    co_bytes = QByteArray::fromRawData((char*)co, sizeof(ControlOut));
    //print(QString(co_bytes.toHex()), PT_INFO);
    emit trySend(co_bytes);

    updateIndicatorValuei(Indicator::DRIVE_FORWARD, co->drive_speed & 0x80 ? 0 : co->drive_speed & 0x7F, 2, "");
    updateIndicatorValuei(Indicator::DRIVE_REVERSE, co->drive_speed & 0x80 ? co->drive_speed & 0x7F : 0, 2, "");
    updateIndicatorValuei(Indicator::YAW_LEFT, co->yaw_speed & 0x80 ? 0 : co->yaw_speed & 0x7F, 2, "");
    updateIndicatorValuei(Indicator::YAW_RIGHT, co->yaw_speed & 0x80 ? co->yaw_speed & 0x7F : 0, 2, "");
    updateIndicatorValuei(Indicator::THROTTLE, axis[AxisAction::THROTTLE]/256, 2, "");

    emit updateIndicatorPanel(ind);

    for (int i = 0; i < ButtonAction::COUNT; i++) {
        jtoggle[i] = 0;
    }
    for (int i = 0; i < ButtonAction::COUNT; i++) {
        if (buttonBindings[i].buttonMode == TOGGLE) {
            button[i] = 0;
        }
    }

    sendControlOut_flag = false;
}

void IOPThread::parseFeedbackIn() {

    if (fi_bytes.length() >= 2) do {
        fi = (FeedbackIn*)(fi_bytes.constData());

        if (fi->device < Device::COUNT) {

            if (fi->status ^ dev[fi->device].status) {
                switch (fi->status) {
                case DeviceStatus::INIT_ERROR:  print(QString("Device %1 could not be initialized.").arg(fi->device), PT_WARNING); break;
                case DeviceStatus::RUN_ERROR:   print(QString("Device %1 has experienced a fault.").arg(fi->device), PT_ERROR); break;
                default: break;
                }
            }
            dev[fi->device].status = fi->status;

            if (fi->device == 255 && fi->data[0] ^ dev[fi->device].data[0]) {
                emit updateVideoPlayer1();
                emit updateVideoPlayer2();
            }
            for (int i = 0; i < dev[fi->device].numParams && i <= fi_bytes.length()/2-2; i++) {
                dev[fi->device].data[i] = fi->data[i];
            }

            fi_bytes.remove(0, 2+2*dev[fi->device].numParams);
        }
        else {
            fi_bytes.clear();
            return; // unknown/corrupt device ID! abort packet
        }
    }
    while (fi_bytes.length() > 0);

    updateIndicatorf(Indicator::BATTERY_1_VOLTS, Device::VOLT_SENSOR_BATTERY, 0, 0.1, 1, " V");
    updateIndicatorValuef(Indicator::BATTERY_1_PERCENT,
            (dev[Device::VOLT_SENSOR_BATTERY].data[0]/10.0-BATTERY_1_MIN_VOLTS)/(BATTERY_1_MAX_VOLTS-BATTERY_1_MIN_VOLTS),
            dev[Device::VOLT_SENSOR_BATTERY].status, 1, "%");
    updateIndicatorf(Indicator::BATTERY_1_TEMP, Device::TEMP_SENSOR_BATTERY, 0, 0.1, 1, "° C");

    updateIndicatorf(Indicator::MOTOR_LEFT_POSITION, Device::LEFT_DRIVE_ENCODER, 0, 0.1, 1, "°");
    updateIndicatorf(Indicator::MOTOR_LEFT_TEMP, Device::TEMP_SENSOR_LEFT_DRIVE_MOTOR, 0, 0.1, 1, "° C");
    updateIndicatorf(Indicator::MOTOR_RIGHT_POSITION, Device::RIGHT_DRIVE_ENCODER, 0, 0.1, 1, "°");
    updateIndicatorf(Indicator::MOTOR_RIGHT_TEMP, Device::TEMP_SENSOR_RIGHT_DRIVE_MOTOR, 0, 0.1, 1, "° C");

    updateIndicatorf(Indicator::FLIPPER_FRONT_POSITION, Device::FRONT_FLIPPER_ENCODER, 0, 0.1, 1, "°");
    updateIndicatorf(Indicator::FLIPPER_FRONT_TEMP, Device::TEMP_SENSOR_FRONT_FLIPPER_MOTOR, 0, 0.1, 1, "° C");
    updateIndicatorf(Indicator::FLIPPER_BACK_POSITION, Device::BACK_FLIPPER_ENCODER, 0, 0.1, 1, "°");
    updateIndicatorf(Indicator::FLIPPER_BACK_TEMP, Device::TEMP_SENSOR_BACK_FLIPPER_MOTOR, 0, 0.1, 1, "° C");

    updateIndicatorf(Indicator::AXIS_ACCEL_X, Device::AGM_BNO055, 0, 0.01, 2, " m/s²");
    updateIndicatorf(Indicator::AXIS_ACCEL_Y, Device::AGM_BNO055, 1, 0.01, 2, " m/s²");
    updateIndicatorf(Indicator::AXIS_ACCEL_Z, Device::AGM_BNO055, 2, 0.01, 2, " m/s²");
    updateIndicatorf(Indicator::EULER_ANGLE_1, Device::AGM_BNO055, 3, 0.1, 1, "°");
    updateIndicatorf(Indicator::EULER_ANGLE_2, Device::AGM_BNO055, 4, 0.1, 1, "°");
    updateIndicatorf(Indicator::EULER_ANGLE_3, Device::AGM_BNO055, 5, 0.1, 1, "°");

    updateIndicatorf(Indicator::G15_BOTTOM_POSITION, Device::MANIPULATOR_ARM, 0, 0.1, 1, "°");
    updateIndicatorf(Indicator::G15_MIDDLE_POSITION, Device::MANIPULATOR_ARM, 1, 0.1, 1, "°");
    updateIndicatorf(Indicator::G15_TOP_POSITION, Device::MANIPULATOR_ARM, 2, 0.1, 1, "°");
    updateIndicatorf(Indicator::G15_PAN_POSITION, Device::MANIPULATOR_ARM, 3, 0.1, 1, "°");

    // OLD Value calculations
    ind[Indicator::CAMERA_1_STREAMING].i = dev[Device::VIDEO_SERVER].data[0] & (1 << 0);
    ind[Indicator::CAMERA_2_STREAMING].i = dev[Device::VIDEO_SERVER].data[0] & (1 << 1);
    ind[Indicator::CAMERA_1_CONNECTED].i = dev[Device::VIDEO_SERVER].data[0] & (1 << 2);
    ind[Indicator::CAMERA_2_CONNECTED].i = dev[Device::VIDEO_SERVER].data[0] & (1 << 3);

    // OLD String constructors
    ind[Indicator::CAMERA_1_STREAMING].str = ind[Indicator::CAMERA_1_STREAMING].i ? QString("ON") : QString("OFF");
    ind[Indicator::CAMERA_2_STREAMING].str = ind[Indicator::CAMERA_2_STREAMING].i ? QString("ON") : QString("OFF");
    ind[Indicator::CAMERA_1_CONNECTED].str = ind[Indicator::CAMERA_1_CONNECTED].i ? QString("CONNECTED") : QString("DISCONNECTED");
    ind[Indicator::CAMERA_2_CONNECTED].str = ind[Indicator::CAMERA_2_CONNECTED].i ? QString("CONNECTED") : QString("DISCONNECTED");
    ind[Indicator::LIGHT_ON].str = ind[Indicator::LIGHT_ON].i ? QString("ON") : QString("OFF");

    emit updateIndicatorPanel(ind);
    emit updateVisual3D(ind);

    parseFeedbackIn_flag = false;
}

void IOPThread::updateIndicatorf(int indicator, int device, int data_index, qreal coeff, int decimal_places, QString units) {

    updateIndicatorValuef(indicator, dev[device].data[data_index]*coeff, dev[device].status, decimal_places, units);
}

void IOPThread::updateIndicatorValuef(int indicator, qreal value, quint8 status, int decimal_places, QString units) {

    ind[indicator].f = value;
    ind[indicator].str = QString("%1%2").arg(value, 0, 'f', decimal_places).arg(units);
    ind[indicator].status = status;
}

void IOPThread::updateIndicatori(int indicator, int device, int data_index, QString units) {

    updateIndicatorValuei(indicator, dev[device].data[data_index], dev[device].status, units);
}

void IOPThread::updateIndicatorValuei(int indicator, int value, quint8 status, QString units) {

    ind[indicator].i = value;
    ind[indicator].str = QString("%1%2").arg(value).arg(units);
    ind[indicator].status = status;
}

void IOPThread::print(QString text, int type) {

    QTime time = QTime::currentTime();
    emit consolePrint(text, type);
    emit logPrint(text, type, time);
}

void IOPThread::axisPosition(int axis, qint16 position) {

    switch (axis) {
    case 0:
        jaxis[AxisControl::X_LEFT] = (quint16)((position < 0) ? (-position-1) : 0);
        jaxis[AxisControl::X_RIGHT] = (quint16)((position > 0) ? (position) : 0);
        break;
    case 1:
        jaxis[AxisControl::Y_FORWARD] = (quint16)((position < 0) ? (-position-1) : 0);
        jaxis[AxisControl::Y_REVERSE] = (quint16)((position > 0) ? (position) : 0);
        break;
    case 2:
        jaxis[AxisControl::Z_CCW] = (quint16)((position < 0) ? (-position-1) : 0);
        jaxis[AxisControl::Z_CW] = (quint16)((position > 0) ? (position) : 0);
        break;
    case 3:
        jaxis[AxisControl::THROTTLE] = (quint16)((-position/2)+16383);
        break;
    default:
        break;
    }
}

void IOPThread::hatState(int hat, quint8 state) {

    switch (hat) {
    case 0:
        jpersist[ButtonControl::HAT_LEFT] = 0x08 & state;
        jpersist[ButtonControl::HAT_RIGHT] = 0x02 & state;
        jpersist[ButtonControl::HAT_UP] = 0x01 & state;
        jpersist[ButtonControl::HAT_DOWN] = 0x04 & state;

        if (state) {
            jtoggle[ButtonControl::HAT_LEFT] = 0x08 & state;
            jtoggle[ButtonControl::HAT_RIGHT] = 0x02 & state;
            jtoggle[ButtonControl::HAT_UP] = 0x01 & state;
            jtoggle[ButtonControl::HAT_DOWN] = 0x04 & state;
        }
        break;
    default:
        break;
    }
}

void IOPThread::buttonState(int button, quint8 state) {

    jpersist[ButtonControl::BUTTON_1 + button] = state;
    if (state){
        jtoggle[ButtonControl::BUTTON_1 + button] = state;
    }
}

void IOPThread::sendTimerExpired() {

    sendControlOut_flag = true;
}

void IOPThread::trySendResult(int success) {

    if (success) {
        // send succeeded
    }
    else {
        // send failed
    }
}

void IOPThread::dataReceived(QByteArray data) {

    fi_bytes = data;
    parseFeedbackIn_flag = true;
}

void IOPThread::settingsChanged() {

    initJoystickReadings();
    initJoystickActions();
}
