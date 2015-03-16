#include "ui_connector.h"

Connector::Connector() {

    layout = new QBoxLayout(QBoxLayout::LeftToRight);

    indicator = new IconIndicator(":/images/green_light.png", ":/images/red_light.png");
    indicator->setFixedWidth(40);
    indicator->setContentsMargins(12, 0, 4, 0);
    indicator->off();

    button = new QPushButton("Connect");
    button->setFixedWidth(100);
    buttonStatus = CBS_CONNECT;

    layout->addWidget(indicator);
    layout->addWidget(button);

    connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}
Connector::~Connector() {}

void Connector::buttonClicked() {

    switch (buttonStatus) {

    case CBS_CONNECT:

        button->setDisabled(true);
        button->setText("Connecting...");
        buttonStatus = CBS_CONNECTING;
        emit tryConnect();

        break;

    case CBS_DISCONNECT:

        button->setDisabled(true);
        button->setText("Disconnecting...");
        emit tryDisconnect();

        break;

    default:
        break;
    }
}

void Connector::tryConnectResult(int success) {

    button->setEnabled(true);

    if (success) {
        button->setText("Disconnect");
        buttonStatus = CBS_DISCONNECT;
        indicator->on();
        emit getConsoleFocus();
    }
    else {
        button->setText("Connect");
        buttonStatus = CBS_CONNECT;
        indicator->off();
    }
}

void Connector::tryDisconnectResult(int success) {

    button->setEnabled(true);

    if (success) {
        button->setText("Connect");
        buttonStatus = CBS_CONNECT;
        indicator->off();
    }
    else {
        button->setText("Disconnect");
        buttonStatus = CBS_DISCONNECT;
        indicator->on();
    }
}
