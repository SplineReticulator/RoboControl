#include "ui_console.h"

Console::Console(QString title) {

    group = new QGroupBox(title);
    layout = new QBoxLayout(QBoxLayout::TopToBottom);

    output = new QTextEdit();
    output->setFont(QFont("Courier", 8));
    output->setReadOnly(true);
    output->setTextInteractionFlags(Qt::NoTextInteraction);
    output->setWordWrapMode(QTextOption::NoWrap);
    output->setMinimumWidth(400);

    input = new QLineEdit();
    input->installEventFilter(this);
    input->setFont(QFont("Courier", 8));
    input->setMinimumWidth(400);

    layout->addWidget(output);
    layout->addWidget(input);

    group->setLayout(layout);

    inputHistoryIndex = -1;
}
Console::~Console() {}

bool Console::eventFilter(QObject* watched, QEvent* event) {

    if (event->type() == QEvent::KeyPress) {

        QLineEdit* input = qobject_cast<QLineEdit*>(watched);
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        switch (keyEvent->key()) {

        case Qt::Key_Up:
            if (inputHistoryIndex+1 < inputHistory.length()) {
                inputHistoryIndex++;
                input->setText(inputHistory[inputHistoryIndex]);
            }
            return true;

        case Qt::Key_Down:
            if (inputHistoryIndex-1 > -1) {
                inputHistoryIndex--;
                input->setText(inputHistory[inputHistoryIndex]);
            }
            else if (inputHistoryIndex == 0) {
                inputHistoryIndex = -1;
                input->setText("");
            }
            return true;

        case Qt::Key_Return:
            if (input->text().length() > 0) {
                print(input->text(), PT_INPUT);
                command(input->text());
                inputHistory.prepend(input->text());
                while (inputHistory.length() > MAX_CONSOLE_HISTORY) inputHistory.removeLast();
                inputHistoryIndex = -1;
                input->clear();
            }
            return true;

        default:
            return false;
        }
    }
    else {
        return false;
    }
}

void Console::print(QString text, int type) {

    switch (type) {

    case PT_ERROR:
        output->setTextColor(QColor(240, 0, 0));
        text.prepend("ERROR: ");
        break;

    case PT_WARNING:
        output->setTextColor(QColor(240, 160, 0));
        text.prepend("WARNING: ");
        break;

    case PT_SUCCESS:
        output->setTextColor(QColor(0, 160, 0));
        break;

    case PT_INFO:
        output->setTextColor(QColor(0, 0, 0));
        break;

    case PT_INPUT:
        output->setTextColor(QColor(0, 0, 240));
        text.prepend(">> ");
        break;

    case PT_DEBUG:
        output->setTextColor(QColor(0, 0, 0));
        break;

    default:
        output->setTextColor(QColor(0, 0, 0));
        break;
    }

    if (type != PT_DEBUG || DEBUG_TRACE) {
        output->append(text);
    }
}

void Console::clear() {

    output->clear();
}

void Console::command(QString text) {

    QString cmd = text.section(' ', 0, 0);

    if (cmd.length() > 0) {

        if (cmd == "send") {
            QString data = text.section(' ', 1, 1);
            if (data.length() > 0) emit command_send(data);
            else emit command_send();
        }
        else if (cmd == "clear") {
            clear();
        }
        else if (cmd == "jaxis") {
            int axis = text.section(' ', 1, 1).toInt();
            qint16 position = text.section(' ', 2, 2).toInt();
            emit command_jaxis(axis, position);
        }
        else if (cmd == "jhat") {
            int hat = text.section(' ', 1, 1).toInt();
            quint8 state = text.section(' ', 2, 2).toUInt();
            emit command_jhat(hat, state);
        }
        else if (cmd == "jbutton") {
            int button = text.section(' ', 1, 1).toInt();
            quint8 state = text.section(' ', 2, 2).toUInt();
            emit command_jbutton(button, state);
        }
        else if (cmd == "resetcam1") {
            emit command_resetcam1();
        }
        else if (cmd == "resetcam2") {
            emit command_resetcam2();
        }
        else {
            print(QString("Unrecognized command '%1'").arg(cmd), PT_ERROR);
        }
    }
}
