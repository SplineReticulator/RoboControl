#include "log.h"

Log::Log() {

    openFile();
}

void Log::openFile() {

    QString name = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss.log").remove(QChar('-'));

    file.setFileName(QString("%1/%2").arg(LOG_PATH).arg(name));
    if (file.open(QIODevice::WriteOnly)) {
        ts.setDevice(&file);
    }
    else {
        // file access error
    }
}

void Log::print(QString text, int type, QTime time) {

    QString stamp = time.toString("[hh:mm:ss.zzz]");

    switch (type) {

    case PT_ERROR:
        text.prepend("ERROR: ");
        break;

    case PT_WARNING:
        text.prepend("WARNING: ");
        break;

    default:
        break;
    }

    ts << stamp << " " << text << "\r\n";

    ts.flush();
}

void Log::close() {

    if (file.isOpen()) {
        file.close();
    }
}
