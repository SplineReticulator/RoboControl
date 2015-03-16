#include "dialog_joystick.h"

JoystickDialog::JoystickDialog(IOPThread* iopt, SDLThread* sdlt) {

    this->iopt = iopt;
    this->sdlt = sdlt;

    layout = new QGridLayout();

    scrollArea = new QScrollArea();
    scrollAreaWidget = new QWidget();
    axisGroup = new QGroupBox("Axes");
    buttonGroup = new QGroupBox("Buttons");

    headerLayout = new QGridLayout();
    scrollAreaLayout = new QGridLayout();
    axisLayout = new QGridLayout();
    buttonLayout = new QGridLayout();
    okCancelLayout = new QGridLayout();

    sdlt->updateJoystickList();
    joystickLabel = new QLabel("Joystick:");
    joystickLabel->setFixedWidth(70);
    joystickSelector = new QComboBox();
    joystickSelector->addItems(sdlt->joystickList);
    joystickSelector->installEventFilter(this);
    joystickSelector->setFixedWidth(300);

    modifierLabel = new QLabel("Use modifier:");
    modifierLabel->setFixedWidth(70);
    modifierEnable = new QCheckBox(" ");
    modifierEnable->setCheckState((iopt->modifierEnable) ? Qt::Checked : Qt::Unchecked);
    modifierEnable->setFixedWidth(20);
    modifierSelector = new QComboBox();
    modifierSelector->addItems(gs.ButtonControl_Names);
    modifierSelector->setCurrentIndex(iopt->modifierButton);
    modifierSelector->installEventFilter(this);
    modifierSelector->setEnabled(iopt->modifierEnable);
    modifierSelector->setFixedWidth(150);

    headerLayout->addWidget(joystickLabel, 0, 0, 1, 1);
    headerLayout->addWidget(joystickSelector, 0, 1, 1, 2);
    headerLayout->addWidget(modifierLabel, 1, 0, 1, 1);
    headerLayout->addWidget(modifierEnable, 1, 1, 1, 1, Qt::AlignVCenter);
    headerLayout->addWidget(modifierSelector, 1, 2, 1, 1, Qt::AlignLeft);

    axisActionLabels = new QLabel*[AxisAction::COUNT];
    axisControlSelectors = new QComboBox*[AxisAction::COUNT];
    axisModifierModeSelectors = new QComboBox*[AxisAction::COUNT];
    for (int i = 0; i < AxisAction::COUNT; i++) {
        axisActionLabels[i] = new QLabel(gs.AxisAction_Names[i]);
        axisControlSelectors[i] = new QComboBox();
        axisControlSelectors[i]->addItems(gs.AxisControl_Names);
        axisControlSelectors[i]->setCurrentIndex(iopt->axisBindings[i].control);
        axisControlSelectors[i]->installEventFilter(this);
        axisControlSelectors[i]->setFixedWidth(150);
        axisModifierModeSelectors[i] = new QComboBox();
        axisModifierModeSelectors[i]->addItems(gs.ModifierMode_Names);
        axisModifierModeSelectors[i]->setCurrentIndex(iopt->axisBindings[i].modifierMode);
        axisModifierModeSelectors[i]->installEventFilter(this);
        axisModifierModeSelectors[i]->setEnabled(iopt->modifierEnable);
        axisModifierModeSelectors[i]->setFixedWidth(100);
        axisLayout->addWidget(axisActionLabels[i], i, 0);
        axisLayout->addWidget(axisControlSelectors[i], i, 1);
        axisLayout->addWidget(axisModifierModeSelectors[i], i, 2);
    }
    axisLayout->setColumnMinimumWidth(0, 150);
    axisGroup->setLayout(axisLayout);

    buttonActionLabels = new QLabel*[ButtonAction::COUNT];
    buttonControlSelectors = new QComboBox*[ButtonAction::COUNT];
    buttonButtonModeSelectors = new QComboBox*[ButtonAction::COUNT];
    buttonModifierModeSelectors = new QComboBox*[ButtonAction::COUNT];
    for (int i = 0; i < ButtonAction::COUNT; i++) {
        buttonActionLabels[i] = new QLabel(gs.ButtonAction_Names[i]);
        buttonControlSelectors[i] = new QComboBox();
        buttonControlSelectors[i]->addItems(gs.ButtonControl_Names);
        buttonControlSelectors[i]->setCurrentIndex(iopt->buttonBindings[i].control);
        buttonControlSelectors[i]->installEventFilter(this);
        buttonControlSelectors[i]->setFixedWidth(150);
        buttonButtonModeSelectors[i] = new QComboBox();
        buttonButtonModeSelectors[i]->addItems(gs.ButtonMode_Names);
        buttonButtonModeSelectors[i]->setCurrentIndex(iopt->buttonBindings[i].buttonMode);
        buttonButtonModeSelectors[i]->installEventFilter(this);
        buttonButtonModeSelectors[i]->setFixedWidth(75);
        buttonModifierModeSelectors[i] = new QComboBox();
        buttonModifierModeSelectors[i]->addItems(gs.ModifierMode_Names);
        buttonModifierModeSelectors[i]->setCurrentIndex(iopt->buttonBindings[i].modifierMode);
        buttonModifierModeSelectors[i]->installEventFilter(this);
        buttonModifierModeSelectors[i]->setEnabled(iopt->modifierEnable);
        buttonModifierModeSelectors[i]->setFixedWidth(100);
        buttonLayout->addWidget(buttonActionLabels[i], i, 0);
        buttonLayout->addWidget(buttonControlSelectors[i], i, 1);
        buttonLayout->addWidget(buttonButtonModeSelectors[i], i, 2);
        buttonLayout->addWidget(buttonModifierModeSelectors[i], i, 3);
    }
    buttonLayout->setColumnMinimumWidth(0, 150);
    buttonGroup->setLayout(buttonLayout);

    scrollAreaLayout->addWidget(axisGroup, 0, 0, Qt::AlignLeft);
    scrollAreaLayout->addWidget(buttonGroup, 1, 0, Qt::AlignLeft);
    scrollAreaWidget->setLayout(scrollAreaLayout);
    scrollArea->setWidget(scrollAreaWidget);
    scrollArea->setFixedWidth(550);

    okButton = new QPushButton("OK");
    okButton->setFixedWidth(75);
    cancelButton = new QPushButton("Cancel");
    cancelButton->setFixedWidth(75);

    okCancelLayout->addWidget(okButton, 0, 0);
    okCancelLayout->addWidget(cancelButton, 0, 1);

    layout->addLayout(headerLayout, 0, 0, Qt::AlignLeft);
    layout->addWidget(scrollArea, 1, 0);
    layout->addLayout(okCancelLayout, 2, 0, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(layout);
    setWindowTitle("Joystick Settings");

    connect(modifierEnable, SIGNAL(clicked(bool)), modifierSelector, SLOT(setEnabled(bool)));
    for (int i = 0; i < AxisAction::COUNT; i++) {
        connect(modifierEnable, SIGNAL(clicked(bool)), axisModifierModeSelectors[i], SLOT(setEnabled(bool)));
    }
    for (int i = 0; i < ButtonAction::COUNT; i++) {
        connect(modifierEnable, SIGNAL(clicked(bool)), buttonModifierModeSelectors[i], SLOT(setEnabled(bool)));
    }

    connect(okButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}
JoystickDialog::~JoystickDialog() {}

bool JoystickDialog::eventFilter(QObject* watched, QEvent* event) {

    if (event->type() == QEvent::Wheel) {
        QComboBox* selector = qobject_cast<QComboBox*>(watched);
        return selector && selector->hasFocus();
    }
    else {
        return false;
    }
}

void JoystickDialog::apply() {

    iopt->modifierEnable = (modifierEnable->checkState() == Qt::Checked) ? 1 : 0;
    iopt->modifierButton = modifierSelector->currentIndex();
    for (int i = 0; i < AxisAction::COUNT; i++) {
        iopt->axisBindings[i].control = axisControlSelectors[i]->currentIndex();
        iopt->axisBindings[i].modifierMode = axisModifierModeSelectors[i]->currentIndex();
    }
    for (int i = 0; i < ButtonAction::COUNT; i++) {
        iopt->buttonBindings[i].control = buttonControlSelectors[i]->currentIndex();
        iopt->buttonBindings[i].buttonMode = buttonButtonModeSelectors[i]->currentIndex();
        iopt->buttonBindings[i].modifierMode = buttonModifierModeSelectors[i]->currentIndex();
    }

    emit settingsChanged();
    emit accept();
}
