#include "ui_iconindicator.h"

IconIndicator::IconIndicator(QString onIcon, QString offIcon) {

    onPixmap.load(onIcon);
    offPixmap.load(offIcon);

    setPixmap(offPixmap);
}
IconIndicator::~IconIndicator() {}

void IconIndicator::on() {
    setPixmap(onPixmap);
}

void IconIndicator::off() {
    setPixmap(offPixmap);
}
