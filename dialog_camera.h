#ifndef DIALOG_CAMERA_H
#define DIALOG_CAMERA_H

#include <QDialog>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

#include "UDPmain.h"
#include "global.h"

class CameraDialog : public QDialog {
    Q_OBJECT

public:
    CameraDialog(UDPThread* udpt);
    ~CameraDialog();

    UDPThread* udpt;

    QComboBox* cameraSelector;
    QLineEdit** ipaddrEdit;
    QLineEdit** portEdit;
    QLineEdit** protocolEdit;
    QLineEdit** vencodeEdit;
    QLineEdit** vclockEdit;

public slots:
    void cameraSelected(int cam);
    void apply();

private:
    QGridLayout* layout;

    QGroupBox* sdpGroup;

    QGridLayout* sdpLayout;
    QGridLayout* okCancelLayout;

    QLabel* cameraLabel;
    QLabel* ipaddrLabel;
    QLabel* portLabel;
    QLabel* protocolLabel;
    QLabel* vencodeLabel;
    QLabel* vclockLabel;

    QPushButton* okButton;
    QPushButton* cancelButton;

    GlobalStrings gs;

signals:
    void settingsChanged();

};

#endif // DIALOG_CAMERA_H
