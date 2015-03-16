#include "ui_indicatorpanel.h"

IndicatorGroup::IndicatorGroup(QString title, QString openIcon, QString closedIcon) {

    setTitle(title);
    setCheckable(true);
    setStyleSheet(QString(
        "QGroupBox::indicator {"
             "width: 16px;"
             "height: 16px;"
        "}"

        "QGroupBox::indicator:checked {"
             "image: url(%1);"
        "}"
        "QGroupBox::indicator::unchecked {"
             "image: url(%2);"
        "}").arg(openIcon, closedIcon)
     );

    layout = new QGridLayout();
    layout->setColumnMinimumWidth(0, 95);
    setLayout(layout);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    numIndicators = 0;

    connect(this, SIGNAL(clicked(bool)), this, SLOT(setContentsVisible(bool)));
}
IndicatorGroup::~IndicatorGroup() {}

void IndicatorGroup::setContentsVisible(bool visible) {

    for (int i = 0; i < layout->count(); i++) {
        layout->itemAt(i)->widget()->setVisible(visible);
    }
}

IndicatorPanel::IndicatorPanel() {

    QFont pt7;
    pt7.setPointSize(7);

    indicatorLabels = new QLabel*[Indicator::COUNT];
    textIndicators = new QLineEdit*[Indicator::COUNT];
    barIndicators = new QProgressBar*[Indicator::COUNT];
    for (int i = 0; i < Indicator::COUNT; i++) {
        indicatorLabels[i] = new QLabel(gs.Indicator_Names[i]);
        indicatorLabels[i]->setFont(pt7);
        textIndicators[i] = new QLineEdit();
        textIndicators[i]->setReadOnly(true);
        textIndicators[i]->setFont(pt7);
        textIndicators[i]->setFixedWidth(50);
        barIndicators[i] = new QProgressBar();
        barIndicators[i]->setMaximum(100);
        barIndicators[i]->setMinimum(0);
    }

    // INFO BAR
    infoBarLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    batteryIcon_1 = new IconIndicator(":/images/battery1.png", ":/images/battery1.png");
    batteryIcon_2 = new IconIndicator(":/images/battery2.png", ":/images/battery2.png");
    lightIndicator = new IconIndicator(":/images/light_on.png", ":/images/light_off.png");
    cameraIndicator_1 = new IconIndicator(":/images/camera1_on.png", ":/images/camera1_off.png");
    cameraIndicator_2 = new IconIndicator(":/images/camera2_on.png", ":/images/camera2_off.png");

    infoBarLayout->addWidget(batteryIcon_1);
    infoBarLayout->addWidget(barIndicators[Indicator::BATTERY_1_PERCENT]);
    infoBarLayout->addWidget(batteryIcon_2);
    infoBarLayout->addWidget(barIndicators[Indicator::BATTERY_2_PERCENT]);
    infoBarLayout->addWidget(lightIndicator);
    infoBarLayout->addWidget(cameraIndicator_1);
    infoBarLayout->addWidget(cameraIndicator_2);

    // POWER
    powerGroup = new IndicatorGroup("Power", ":/images/group_power.png", ":/images/group_power.png");
    addTextIndicator(powerGroup, Indicator::BATTERY_1_VOLTS);
    addTextIndicator(powerGroup, Indicator::BATTERY_1_TEMP);
    addTextIndicator(powerGroup, Indicator::BATTERY_2_VOLTS);
    addTextIndicator(powerGroup, Indicator::BATTERY_2_TEMP);

    // DRIVE MOTORS
    driveMotorsGroup = new IndicatorGroup("Drive Motors", ":/images/group_motors.png", ":/images/group_motors.png");
    addTextIndicator(driveMotorsGroup, Indicator::MOTOR_LEFT_POSITION);
    addTextIndicator(driveMotorsGroup, Indicator::MOTOR_LEFT_TEMP);
    addTextIndicator(driveMotorsGroup, Indicator::MOTOR_RIGHT_POSITION);
    addTextIndicator(driveMotorsGroup, Indicator::MOTOR_RIGHT_TEMP);

    // FLIPPERS
    flippersGroup = new IndicatorGroup("Flippers", ":/images/group_flippers.png", ":/images/group_flippers.png");
    addTextIndicator(flippersGroup, Indicator::FLIPPER_FRONT_POSITION);
    addTextIndicator(flippersGroup, Indicator::FLIPPER_FRONT_TEMP);
    addTextIndicator(flippersGroup, Indicator::FLIPPER_BACK_POSITION);
    addTextIndicator(flippersGroup, Indicator::FLIPPER_BACK_TEMP);

    // ORIENTATION
    orientationGroup = new IndicatorGroup("Orientation", ":/images/group_orientation.png", ":/images/group_orientation.png");
    addTextIndicator(orientationGroup, Indicator::AXIS_ACCEL_X);
    addTextIndicator(orientationGroup, Indicator::AXIS_ACCEL_Y);
    addTextIndicator(orientationGroup, Indicator::AXIS_ACCEL_Z);
    addTextIndicator(orientationGroup, Indicator::EULER_ANGLE_1);
    addTextIndicator(orientationGroup, Indicator::EULER_ANGLE_2);
    addTextIndicator(orientationGroup, Indicator::EULER_ANGLE_3);

    // MANIPULATOR ARM
    manipulatorGroup = new IndicatorGroup("Manipulator Arm", ":/images/group_manipulator.png", ":/images/group_manipulator.png");
    addTextIndicator(manipulatorGroup, Indicator::G15_BOTTOM_POSITION);
    addTextIndicator(manipulatorGroup, Indicator::G15_MIDDLE_POSITION);
    addTextIndicator(manipulatorGroup, Indicator::G15_TOP_POSITION);
    addTextIndicator(manipulatorGroup, Indicator::G15_PAN_POSITION);

    // JOYSTICK
    joystickGroup = new IndicatorGroup("Joystick", ":/images/group_joystick.png", ":/images/group_joystick.png");
    addTextIndicator(joystickGroup, Indicator::DRIVE_FORWARD);
    addTextIndicator(joystickGroup, Indicator::DRIVE_REVERSE);
    addTextIndicator(joystickGroup, Indicator::YAW_LEFT);
    addTextIndicator(joystickGroup, Indicator::YAW_RIGHT);
    addTextIndicator(joystickGroup, Indicator::THROTTLE);

    errorLinesTimer.setInterval(1000);
    errorLinesTimer.start();
    connect(&errorLinesTimer, SIGNAL(timeout()), this, SLOT(toggleErrorLines()));

    showErrorLines = true;
}

IndicatorPanel::~IndicatorPanel() {}

void IndicatorPanel::addTextIndicator(IndicatorGroup* group, int indicator) {

    group->layout->addWidget(indicatorLabels[indicator], group->numIndicators, 0);
    group->layout->addWidget(textIndicators[indicator], group->numIndicators, 1);
    group->numIndicators++;
}

void IndicatorPanel::update(Indicator* ind) {

    for (int i = 0; i < Indicator::COUNT; i++) {
        switch (ind[i].status) {
        case DeviceStatus::RUNNING:
            textIndicators[i]->setText(ind[i].str);
            textIndicators[i]->setToolTip("");
            break;
        case DeviceStatus::INIT_ERROR:
            textIndicators[i]->setText("----------");
            textIndicators[i]->setToolTip("Device initialization error");
            break;
        case DeviceStatus::RUN_ERROR:
            textIndicators[i]->setText(showErrorLines ? "----------" : ind[i].str);
            textIndicators[i]->setToolTip("Device fault");
            break;
        default:
            textIndicators[i]->setText(ind[i].str);
            break;
        }
        barIndicators[i]->setValue(qMax(qMin(qRound(ind[i].f*100),100),0));
    }

    ind[Indicator::LIGHT_ON].i ? lightIndicator->on() : lightIndicator->off();
    (ind[Indicator::CAMERA_1_STREAMING].i && ind[Indicator::CAMERA_1_CONNECTED].i) ? cameraIndicator_1->on() : cameraIndicator_1->off();
    (ind[Indicator::CAMERA_2_STREAMING].i && ind[Indicator::CAMERA_2_CONNECTED].i) ? cameraIndicator_2->on() : cameraIndicator_2->off();
}

void IndicatorPanel::toggleErrorLines() {

    showErrorLines = !showErrorLines;
}
