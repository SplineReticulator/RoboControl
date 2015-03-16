#include "global.h"

GlobalStrings::GlobalStrings() {

    AxisControl_Names.append("");
    AxisControl_Names.append("X_AXIS_LEFT");
    AxisControl_Names.append("X_AXIS_RIGHT");
    AxisControl_Names.append("Y_AXIS_FORWARD");
    AxisControl_Names.append("Y_AXIS_BACK");
    AxisControl_Names.append("Z_AXIS_CCW");
    AxisControl_Names.append("Z_AXIS_CW");
    AxisControl_Names.append("THROTTLE");

    ButtonControl_Names.append("");
    ButtonControl_Names.append("BUTTON_1");
    ButtonControl_Names.append("BUTTON_2");
    ButtonControl_Names.append("BUTTON_3");
    ButtonControl_Names.append("BUTTON_4");
    ButtonControl_Names.append("BUTTON_5");
    ButtonControl_Names.append("BUTTON_6");
    ButtonControl_Names.append("BUTTON_7");
    ButtonControl_Names.append("BUTTON_8");
    ButtonControl_Names.append("BUTTON_9");
    ButtonControl_Names.append("BUTTON_10");
    ButtonControl_Names.append("BUTTON_11");
    ButtonControl_Names.append("BUTTON_12");
    ButtonControl_Names.append("HAT_LEFT");
    ButtonControl_Names.append("HAT_RIGHT");
    ButtonControl_Names.append("HAT_UP");
    ButtonControl_Names.append("HAT_DOWN");

    AxisAction_Names.append("DRIVE_FORWARD");
    AxisAction_Names.append("DRIVE_REVERSE");
    AxisAction_Names.append("YAW_LEFT");
    AxisAction_Names.append("YAW_RIGHT");
    AxisAction_Names.append("THROTTLE");

    ButtonAction_Names.append("FRONT_FLIP_FORWARD");
    ButtonAction_Names.append("FRONT_FLIP_REVERSE");
    ButtonAction_Names.append("BACK_FLIP_FORWARD");
    ButtonAction_Names.append("BACK_FLIP_REVERSE");
    ButtonAction_Names.append("ARM_RAISE");
    ButtonAction_Names.append("ARM_LOWER");
    ButtonAction_Names.append("PAN_LEFT");
    ButtonAction_Names.append("PAN_RIGHT");
    ButtonAction_Names.append("TILT_UP");
    ButtonAction_Names.append("TILT_DOWN");
    ButtonAction_Names.append("LIGHTS_ON_OFF");
    ButtonAction_Names.append("FRONT_CAM_POWER");
    ButtonAction_Names.append("REAR_CAM_POWER");
    ButtonAction_Names.append("EMERGENCY_STOP");
    ButtonAction_Names.append("SHUTDOWN");

    ButtonMode_Names.append("Persist");
    ButtonMode_Names.append("Toggle");

    ModifierMode_Names.append("Modified");
    ModifierMode_Names.append("Always");
    ModifierMode_Names.append("Unmodified");

    Indicator_Names.append("BATTERY_1_VOLTS");
    Indicator_Names.append("BATTERY_1_PERCENT");
    Indicator_Names.append("BATTERY_1_TEMP");
    Indicator_Names.append("BATTERY_2_VOLTS");
    Indicator_Names.append("BATTERY_2_PERCENT");
    Indicator_Names.append("BATTERY_2_TEMP");
    Indicator_Names.append("LEFT_POSITION");
    Indicator_Names.append("LEFT_TEMP");
    Indicator_Names.append("RIGHT_POSITION");
    Indicator_Names.append("RIGHT_TEMP");
    Indicator_Names.append("FRONT_POSITION");
    Indicator_Names.append("FRONT_TEMP");
    Indicator_Names.append("BACK_POSITION");
    Indicator_Names.append("BACK_TEMP");
    Indicator_Names.append("AXIS_ACCEL_X");
    Indicator_Names.append("AXIS_ACCEL_Y");
    Indicator_Names.append("AXIS_ACCEL_Z");
    Indicator_Names.append("EULER_ANGLE_1");
    Indicator_Names.append("EULER_ANGLE_2");
    Indicator_Names.append("EULER_ANGLE_3");
    Indicator_Names.append("SERVO_BOTTOM");
    Indicator_Names.append("SERVO_MIDDLE");
    Indicator_Names.append("SERVO_TOP");
    Indicator_Names.append("SERVO_PAN");
    Indicator_Names.append("DRIVE_FORWARD");
    Indicator_Names.append("DRIVE_REVERSE");
    Indicator_Names.append("YAW_LEFT");
    Indicator_Names.append("YAW_RIGHT");
    Indicator_Names.append("THROTTLE");
    Indicator_Names.append("CAM_1_STREAMING");
    Indicator_Names.append("CAM_2_STREAMING");
    Indicator_Names.append("CAM_1_CONNECTED");
    Indicator_Names.append("CAM_2_CONNECTED");
    Indicator_Names.append("LIGHT_ON");

    Camera_Names.append("FRONT");
    Camera_Names.append("REAR");
}
