#include "dialog_network.h"

NetworkDialog::NetworkDialog(TCPThread* tcpt) {

    this->tcpt = tcpt;

    layout = new QGridLayout();

    tcpGroup = new QGroupBox("TCP Server");

    tcpGroupLayout = new QGridLayout();
    okCancelLayout = new QGridLayout();

    ipaddrLabel = new QLabel("IP Address:");
    ipaddrEdit = new QLineEdit(tcpt->ipaddr);
    ipaddrEdit->setInputMask("000.000.000.000;_");
    ipaddrEdit->setFixedWidth(100);

    portLabel = new QLabel("Port:");
    portEdit = new QLineEdit(QString("%1").arg(tcpt->port));
    portEdit->setMaxLength(5);
    portEdit->setFixedWidth(50);

    tcpGroupLayout->addWidget(ipaddrLabel, 0, 0);
    tcpGroupLayout->addWidget(ipaddrEdit, 0, 1);
    tcpGroupLayout->addWidget(portLabel, 1, 0);
    tcpGroupLayout->addWidget(portEdit, 1, 1);
    tcpGroupLayout->setColumnMinimumWidth(0, 75);
    tcpGroup->setLayout(tcpGroupLayout);

    okButton = new QPushButton("OK");
    okButton->setFixedWidth(75);
    cancelButton = new QPushButton("Cancel");
    cancelButton->setFixedWidth(75);

    okCancelLayout->addWidget(okButton, 0, 0);
    okCancelLayout->addWidget(cancelButton, 0, 1);

    layout->addWidget(tcpGroup, 0, 0);
    layout->addLayout(okCancelLayout, 1, 0, Qt::AlignRight);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(layout);
    setWindowTitle("Network Settings");

    connect(okButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}
NetworkDialog::~NetworkDialog() {}

void NetworkDialog::apply() {

    tcpt->ipaddr = ipaddrEdit->text();
    tcpt->port = portEdit->text().toUInt();

    emit settingsChanged();
    emit accept();
}
