#ifndef SYSDEF_H
#define SYSDEF_H

#include <QString>
#include <QStringList>

#define DEBUG_TRACE 1

// FILES
#define INI_FILE "setup.ini"
#define LOG_PATH "logs"

// BUFFERS
#define MAX_CONSOLE_HISTORY 50
#define MAX_DEVICE_PARAMS 8

// TIMING
#define CONTROLOUT_SEND_FREQ 10
#define IOPROCESS_FREQ 100
#define JOYSTICK_READ_FREQ 100
#define TCP_UPDATE_FREQ 1000
#define UDP_UPDATE_FREQ 1000

#define CONTROLOUT_TIMER_MS 1000/CONTROLOUT_SEND_FREQ
#define IOPT_USLEEP 1000000/IOPROCESS_FREQ
#define SDLT_USLEEP 1000000/JOYSTICK_READ_FREQ
#define TCPT_USLEEP 1000000/TCP_UPDATE_FREQ
#define UDPT_USLEEP 1000000/UDP_UPDATE_FREQ

#define CONN_TIMEOUT_MS 3000
#define WAIT_BYTESWRITTEN_MS 1
#define WAIT_READYREAD_MS 1

#define RTP_REFRESH_INTERVAL 20

// INDICATOR RANGES
#define BATTERY_1_MAX_VOLTS 29.4
#define BATTERY_1_MIN_VOLTS 17.5
#define BATTERY_2_MAX_VOLTS 12.6
#define BATTERY_2_MIN_VOLTS 9.0

#define MOTOR_MAX_TEMP 50.0
#define MOTOR_MIN_TEMP 0.0

enum SystemState {
    SS_INIT,
    SS_RUNNING,
    SS_CLOSING
};

enum ConnectionState {
    CS_DISCONNECTED,
    CS_CONNECTING,
    CS_CONNECTED,
    CS_DISCONNECTING
};

enum WriteState {
    WS_IDLE,
    WS_READY,
    WS_BUSY
};

enum ConnectionButtonStatus {
    CBS_CONNECT,
    CBS_CONNECTING,
    CBS_DISCONNECT
};

enum PrintType {
    PT_ERROR,
    PT_WARNING,
    PT_SUCCESS,
    PT_INFO,
    PT_INPUT,
    PT_DEBUG
};

struct AxisControl {
    enum {
        NOAXIS,
        X_LEFT,
        X_RIGHT,
        Y_FORWARD,
        Y_REVERSE,
        Z_CCW,
        Z_CW,
        THROTTLE,
        /*===COUNT===*/
        COUNT
    };
};

struct ButtonControl {
    enum {
        NOBUTTON,
        BUTTON_1,
        BUTTON_2,
        BUTTON_3,
        BUTTON_4,
        BUTTON_5,
        BUTTON_6,
        BUTTON_7,
        BUTTON_8,
        BUTTON_9,
        BUTTON_10,
        BUTTON_11,
        BUTTON_12,
        HAT_LEFT,
        HAT_RIGHT,
        HAT_UP,
        HAT_DOWN,
        /*===COUNT===*/
        COUNT
    };
};

struct AxisAction {
    enum {
        DRIVE_FORWARD,
        DRIVE_REVERSE,
        YAW_LEFT,
        YAW_RIGHT,
        THROTTLE,
        /*===COUNT===*/
        COUNT
    };
};

struct ButtonAction {
    enum {
        FRONT_FLIP_FORWARD,
        FRONT_FLIP_REVERSE,
        BACK_FLIP_FORWARD,
        BACK_FLIP_REVERSE,
        ARM_RAISE,
        ARM_LOWER,
        PAN_LEFT,
        PAN_RIGHT,
        TILT_UP,
        TILT_DOWN,
        LIGHTS_ON_OFF,
        FRONT_CAM_POWER,
        REAR_CAM_POWER,
        EMERGENCY_STOP,
        SHUTDOWN,
        /*===COUNT===*/
        COUNT
    };
};

enum ButtonMode {
    PERSIST = 0,
    TOGGLE = 1
};

enum ModifierMode {
    MODIFIED = 0,
    ALWAYS = 1,
    UNMODIFIED = 2
};

struct AxisBinding {
    int control;
    int modifierMode;
};

struct ButtonBinding {
    int control;
    int modifierMode;
    int buttonMode;
};

#pragma pack(push, 1)
struct ControlOut {

    quint8 drive_speed;     // bit 7:   direction (0=forward, 1=reverse)
                            // bit 6-0: speed (0-127)

    quint8 yaw_speed;       // bit 7:   direction (0=left, 1=right)
                            // bit 6-0: speed (0-127)

    quint8 flippers;        // bit 7:   front reverse
                            // bit 6:   front forward
                            // bit 5:   back reverse
                            // bit 4:   back forward
                            // bit 3-0: don't care

    quint8 arm_lights;      // bit 7:   arm lower
                            // bit 6:   arm raise
                            // bit 5:   pan right
                            // bit 4:   pan left
                            // bit 3:   tilt down
                            // bit 2:   tilt up
                            // bit 1:   lights on/off
                            // bit 0:   lights on/off

    quint8 cameras_stop;    // bit 7-6: emergency stop
                            // bit 5-4: shutdown
                            // bit 3:   front camera on/off
                            // bit 2:   rear camera on/off
                            // bit 1-0: don't care
};
#pragma pack(pop)

struct Device {

    quint8 numParams;
    quint8 status;
    qint16 data[MAX_DEVICE_PARAMS];

    enum {
        // ENCODERS
        RIGHT_DRIVE_ENCODER = 01,
        LEFT_DRIVE_ENCODER = 02,
        FRONT_FLIPPER_ENCODER = 03,
        BACK_FLIPPER_ENCODER = 04,
        // AGM
        AGM_BNO055 = 11,
        // MOTOR CONTROLLERS
        DRIVE_MOTOR_CONTROLLER = 21,
        FLIPPER_MOTOR_CONTROLLER = 22,
        // IPC
        INTERPROCESSOR_COM_HANDLER = 31,
        // SERVOS
        G15_SERVO_BOTTOM = 41,
        G15_SERVO_MIDDLE = 42,
        G15_SERVO_TOP = 43,
        G15_SERVO_PAN = 44,
        // MANIPULATOR ARM
        MANIPULATOR_ARM = 51,
        // TEMPERATURE SENSORS
        TEMP_SENSOR_BATTERY = 61,
        TEMP_SENSOR_LEFT_DRIVE_MOTOR = 62,
        TEMP_SENSOR_RIGHT_DRIVE_MOTOR = 63,
        TEMP_SENSOR_FRONT_FLIPPER_MOTOR = 64,
        TEMP_SENSOR_BACK_FLIPPER_MOTOR = 65,
        // VOLTAGE SENSORS
        VOLT_SENSOR_BATTERY = 71,
        // LINUX OS
        VIDEO_SERVER = 255,
        /*===COUNT===*/
        COUNT
    };
};

struct DeviceStatus {
    enum {
        UNINITIALIZED = 01,
        RUNNING = 02,
        INIT_ERROR = 03,
        RUN_ERROR = 04
    };
};

struct Indicator {

    int i;
    qreal f;
    QString str;
    quint8 status;

    enum {
        // POWER
        BATTERY_1_VOLTS,
        BATTERY_1_PERCENT,
        BATTERY_1_TEMP,
        BATTERY_2_VOLTS,
        BATTERY_2_PERCENT,
        BATTERY_2_TEMP,
        // DRIVE MOTORS
        MOTOR_LEFT_POSITION,
        MOTOR_LEFT_TEMP,
        MOTOR_RIGHT_POSITION,
        MOTOR_RIGHT_TEMP,
        // FLIPPER MOTORS
        FLIPPER_FRONT_POSITION,
        FLIPPER_FRONT_TEMP,
        FLIPPER_BACK_POSITION,
        FLIPPER_BACK_TEMP,
        // AGM
        AXIS_ACCEL_X,
        AXIS_ACCEL_Y,
        AXIS_ACCEL_Z,
        EULER_ANGLE_1,
        EULER_ANGLE_2,
        EULER_ANGLE_3,
        // SERVOS
        G15_BOTTOM_POSITION,
        G15_MIDDLE_POSITION,
        G15_TOP_POSITION,
        G15_PAN_POSITION,
        // JOYSTICK
        DRIVE_FORWARD,
        DRIVE_REVERSE,
        YAW_LEFT,
        YAW_RIGHT,
        THROTTLE,
        // PERIPHERALS
        CAMERA_1_STREAMING,
        CAMERA_2_STREAMING,
        CAMERA_1_CONNECTED,
        CAMERA_2_CONNECTED,
        LIGHT_ON,
        /*===COUNT===*/
        COUNT
    };
};

#pragma pack(push, 1)
struct FeedbackIn {
    quint8 device;
    quint8 status;
    qint16 data[MAX_DEVICE_PARAMS];
};
#pragma pack(pop)

struct Camera {
    enum {
        FRONT,
        REAR,
        /*===COUNT===*/
        COUNT
    };
};

struct GlobalStrings {

    GlobalStrings();

    QStringList AxisControl_Names;
    QStringList ButtonControl_Names;
    QStringList AxisAction_Names;
    QStringList ButtonAction_Names;
    QStringList ButtonMode_Names;
    QStringList ModifierMode_Names;
    QStringList Indicator_Names;
    QStringList Camera_Names;
};

#endif // SYSDEF_H
