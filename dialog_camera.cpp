#include "dialog_camera.h"

CameraDialog::CameraDialog(UDPThread* udpt) {

    this->udpt = udpt;

    layout = new QGridLayout();

    sdpGroup = new QGroupBox("SDP");

    sdpLayout = new QGridLayout();
    okCancelLayout = new QGridLayout();

    cameraLabel = new QLabel("Camera:");
    sdpLayout->addWidget(cameraLabel, 0, 0);
    cameraSelector = new QComboBox();
    cameraSelector->addItems(gs.Camera_Names);
    cameraSelector->setCurrentIndex(0);
    cameraSelector->setFixedWidth(100);
    sdpLayout->addWidget(cameraSelector, 0, 1);

    ipaddrLabel = new QLabel("IP Address:");
    sdpLayout->addWidget(ipaddrLabel, 1, 0);
    ipaddrEdit = new QLineEdit*[Camera::COUNT];
    for (int i = 0; i < Camera::COUNT; i++) {
        ipaddrEdit[i] = new QLineEdit(udpt->streams[i].ip_addr);
        ipaddrEdit[i]->setInputMask("000.000.000.000;_");
        ipaddrEdit[i]->setFixedWidth(100);
        ipaddrEdit[i]->hide();
        sdpLayout->addWidget(ipaddrEdit[i], 1, 1);
    }

    portLabel = new QLabel("Port:");
    sdpLayout->addWidget(portLabel, 2, 0);
    portEdit = new QLineEdit*[Camera::COUNT];
    for (int i = 0; i < Camera::COUNT; i++) {
        portEdit[i] = new QLineEdit(QString("%1").arg(udpt->streams[i].port));
        portEdit[i]->setMaxLength(5);
        portEdit[i]->setFixedWidth(50);
        portEdit[i]->hide();
        sdpLayout->addWidget(portEdit[i], 2, 1);
    }

    protocolLabel = new QLabel("Protocol:");
    sdpLayout->addWidget(protocolLabel, 3, 0);
    protocolEdit = new QLineEdit*[Camera::COUNT];
    for (int i = 0; i < Camera::COUNT; i++) {
        protocolEdit[i] = new QLineEdit(udpt->streams[i].protocol);
        protocolEdit[i]->setFixedWidth(100);
        protocolEdit[i]->hide();
        sdpLayout->addWidget(protocolEdit[i], 3, 1);
    }

    vencodeLabel = new QLabel("Video Encode:");
    sdpLayout->addWidget(vencodeLabel, 4, 0);
    vencodeEdit = new QLineEdit*[Camera::COUNT];
    for (int i = 0; i < Camera::COUNT; i++) {
        vencodeEdit[i] = new QLineEdit(udpt->streams[i].video_encode);
        vencodeEdit[i]->setFixedWidth(100);
        vencodeEdit[i]->hide();
        sdpLayout->addWidget(vencodeEdit[i], 4, 1);
    }

    vclockLabel = new QLabel("Video Clock:");
    sdpLayout->addWidget(vclockLabel, 5, 0);
    vclockEdit = new QLineEdit*[Camera::COUNT];
    for (int i = 0; i < Camera::COUNT; i++) {
        vclockEdit[i] = new QLineEdit(QString("%1").arg(udpt->streams[i].video_clock));
        vclockEdit[i]->setMaxLength(6);
        vclockEdit[i]->setFixedWidth(50);
        vclockEdit[i]->hide();
        sdpLayout->addWidget(vclockEdit[i], 5, 1);
    }

    sdpLayout->setColumnMinimumWidth(0, 75);
    sdpGroup->setLayout(sdpLayout);

    okButton = new QPushButton("OK");
    okButton->setFixedWidth(75);
    cancelButton = new QPushButton("Cancel");
    cancelButton->setFixedWidth(75);

    okCancelLayout->addWidget(okButton, 0, 0);
    okCancelLayout->addWidget(cancelButton, 0, 1);

    layout->addWidget(sdpGroup, 0, 0);
    layout->addLayout(okCancelLayout, 1, 0, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(layout);
    setWindowTitle("Camera Settings");

    cameraSelected(0);

    connect(cameraSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(cameraSelected(int)));
    connect(okButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}
CameraDialog::~CameraDialog() {}

void CameraDialog::cameraSelected(int cam) {

    ipaddrEdit[cam]->show();
    portEdit[cam]->show();
    protocolEdit[cam]->show();
    vencodeEdit[cam]->show();
    vclockEdit[cam]->show();

    for (int i = 0; i < Camera::COUNT; i++) {
        if (i != cam) {
            ipaddrEdit[i]->hide();
            portEdit[i]->hide();
            protocolEdit[i]->hide();
            vencodeEdit[i]->hide();
            vclockEdit[i]->hide();
        }
    }
}

void CameraDialog::apply() {

    for (int i = 0; i < Camera::COUNT; i++) {
        udpt->streams[i].ip_addr = ipaddrEdit[i]->text();
        udpt->streams[i].port = portEdit[i]->text().toUInt();
        udpt->streams[i].protocol = protocolEdit[i]->text();
        udpt->streams[i].video_encode = vencodeEdit[i]->text();
        udpt->streams[i].video_clock = vclockEdit[i]->text().toUInt();
    }

    emit settingsChanged();
    emit accept();
}
