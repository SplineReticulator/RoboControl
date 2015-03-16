#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include "global.h"

class Log : public QObject {
    Q_OBJECT

public:
    Log();

    void close();

public slots:
    void print(QString text, int type, QTime time);

private:
    void openFile();

    QFile file;
    QTextStream ts;
};

#endif // LOG_H
