#ifndef DIALOG_JOYSTICK_H
#define DIALOG_JOYSTICK_H

#include <QDialog>
#include <QWidget>
#include <QGroupBox>
#include <QScrollArea>
#include <QGridLayout>
#include <QEvent>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

#include "IOPmain.h"
#include "SDLmain.h"
#include "global.h"

class JoystickDialog : public QDialog {
    Q_OBJECT

public:
    JoystickDialog(IOPThread* iopt, SDLThread* sdlt);
    ~JoystickDialog();

    bool eventFilter(QObject* watched, QEvent* event);

    IOPThread* iopt;
    SDLThread* sdlt;

    QComboBox* joystickSelector;
    QCheckBox* modifierEnable;
    QComboBox* modifierSelector;
    QComboBox** axisControlSelectors;
    QComboBox** axisModifierModeSelectors;
    QComboBox** buttonControlSelectors;
    QComboBox** buttonModifierModeSelectors;
    QComboBox** buttonButtonModeSelectors;

public slots:
    void apply();

private:
    QGridLayout* layout;

    QScrollArea* scrollArea;
    QWidget* scrollAreaWidget;
    QGroupBox* axisGroup;
    QGroupBox* buttonGroup;

    QGridLayout* headerLayout;
    QGridLayout* scrollAreaLayout;
    QGridLayout* axisLayout;
    QGridLayout* buttonLayout;
    QGridLayout* okCancelLayout;

    QLabel* joystickLabel;
    QLabel* modifierLabel;
    QLabel** axisActionLabels;
    QLabel** buttonActionLabels;
    QPushButton* okButton;
    QPushButton* cancelButton;

    GlobalStrings gs;

signals:
    void settingsChanged();

};

#endif // DIALOG_JOYSTICK_H
