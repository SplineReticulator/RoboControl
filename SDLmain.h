#ifndef SDL_MAIN_H
#define SDL_MAIN_H

#include <QThread>
#include <QElapsedTimer>
#include <QTime>

#include "SDL.h"
#include "ui_console.h"
#include "config.h"
#include "global.h"

class SDLThread : public QThread {
    Q_OBJECT

public:
    void init(Config* ini);
    void run();
    void shutdown();
    void poll();

    void updateJoystickList();

    Config* ini;

    QStringList joystickList;

private:
    void print(QString text, int type);

    SystemState state;

    SDL_Joystick* js;

signals:
    void axisPosition(int axis, qint16 position);
    void hatState(int hat, quint8 state);
    void buttonState(int button, quint8 state);

    void setJoystickMenuActionEnabled(bool enabled);

    void consolePrint(QString text, int type);
    void logPrint(QString text, int type, QTime time);

};

#endif // SDL_MAIN_H
