#ifndef DIALOG_NETWORK_H
#define DIALOG_NETWORK_H

#include <QDialog>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "TCPmain.h"
#include "global.h"

class NetworkDialog : public QDialog {
    Q_OBJECT

public:
    NetworkDialog(TCPThread* tcpt);
    ~NetworkDialog();

    TCPThread* tcpt;

    QLineEdit* ipaddrEdit;
    QLineEdit* portEdit;

public slots:
    void apply();

private:
    QGridLayout* layout;

    QGroupBox* tcpGroup;

    QGridLayout* tcpGroupLayout;
    QGridLayout* okCancelLayout;

    QLabel* ipaddrLabel;
    QLabel* portLabel;

    QPushButton* okButton;
    QPushButton* cancelButton;

signals:
    void settingsChanged();

};

#endif // DIALOG_NETWORK_H
