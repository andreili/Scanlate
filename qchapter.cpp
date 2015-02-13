#include "qchapter.h"

QChapter::QChapter(QJsonObject raw_data, QObject *parent) :
    QObject(parent)
{
    this->id = raw_data["id"].toInt();
    this->name = raw_data["name"].toString();
    this->number = raw_data["num"].toInt();
}
