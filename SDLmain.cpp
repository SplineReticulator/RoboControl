#include "SDLmain.h"

void SDLThread::init(Config* ini) {

    state = SS_INIT;

    this->ini = ini;
    js = NULL;

    SDL_version comp;
    SDL_VERSION(&comp);

    int sdlerr = 0;
    sdlerr |= SDL_Init(SDL_INIT_JOYSTICK);

    if (!sdlerr) {
        print(QString("SDL initialized successfully (v%1.%2.%3).").arg((int)comp.major).arg((int)comp.minor).arg((int)comp.patch), PT_INFO);
    }
    else {
        print(QString("SDL initialization failed."), PT_ERROR);
    }
}

void SDLThread::run() {

    if (state != SS_CLOSING) state = SS_RUNNING;

    bool noJoystickWarning = true;

    while (state == SS_RUNNING) {

        if (js == NULL) {

            if (SDL_NumJoysticks() > 0) {

                js = SDL_JoystickOpen(0);
                if (js != NULL && SDL_JoystickNumAxes(js) > 0) {
                    emit setJoystickMenuActionEnabled(1);
                    print(QString("Found %1 joystick(s), using %2 (device 0) by default.").arg(SDL_NumJoysticks()).arg(SDL_JoystickName(js)), PT_INFO);
                }
                else {
                    js = NULL;
                }
            }

            else if (noJoystickWarning) {

                emit setJoystickMenuActionEnabled(0);
                print(QString("No joysticks are attached to the system. Functionality will be limited."), PT_WARNING);
            }

            noJoystickWarning = false;
        }

        else {

            if (SDL_JoystickGetAttached(js)) {

                for (int axis = 0; axis < SDL_JoystickNumAxes(js); axis++) {
                    emit axisPosition(axis, SDL_JoystickGetAxis(js, axis));
                }
                noJoystickWarning = true;
            }

            else if (noJoystickWarning) {

                js = NULL;
                emit setJoystickMenuActionEnabled(0);
                print(QString("Joystick disconnected unexpectedly."), PT_ERROR);
                noJoystickWarning = false;
            }
        }

        poll();

        usleep(SDLT_USLEEP);
    }

    if (SDL_JoystickGetAttached(js)) {
        SDL_JoystickClose(js);
    }

    SDL_Quit();
}

void SDLThread::poll() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_JOYHATMOTION:
            emit hatState(event.jhat.hat, event.jhat.value); break;

        case SDL_JOYBUTTONDOWN:
            emit buttonState(event.jbutton.button, 1); break;

        case SDL_JOYBUTTONUP:
            emit buttonState(event.jbutton.button, 0); break;

        default: break;
        }
    }
}

void SDLThread::shutdown() {

    state = SS_CLOSING;
}

void SDLThread::updateJoystickList() {

    SDL_Joystick* js_temp;

    int numJoysticks = SDL_NumJoysticks();

    joystickList.clear();

    for (int i = 0; i < numJoysticks; i++) {
        js_temp = SDL_JoystickOpen(i);
        if (js_temp != NULL) {
            joystickList.append(QString("Device %1:  %2").arg(i).arg(SDL_JoystickName(js_temp)));
        }
    }
}

void SDLThread::print(QString text, int type) {

    QTime time = QTime::currentTime();
    emit consolePrint(text, type);
    emit logPrint(text, type, time);
}


