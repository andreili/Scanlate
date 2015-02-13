#include "qscanlateproject.h"
#include <QFile>

QScanlateProject::QScanlateProject(QJsonObject raw_data, QObject *parent) :
    QObject(parent)
{
    this->id = raw_data["id"].toInt();
    this->name = raw_data["name"].toString();
    this->lastActivies = raw_data["activies"].toString();
    this->status = (ProjectStatus)raw_data["status"].toInt();
    this->author = raw_data["author"].toString();
    this->releaseDate = raw_data["release_date"].toInt();
    this->description = raw_data["descr"].toString();
    if (raw_data["cover"].toString().isEmpty())
    {
        this->cover = QPixmap::fromImage(QImage(":/images/no-cover.gif"));
    }
    else
    {
        QByteArray cover_data = QByteArray::fromBase64(raw_data["cover"].toString().toLatin1());
        this->cover = QPixmap::fromImage(QImage::fromData(cover_data, "PNG"));
    }
}

void QScanlateProject::addToTable(QTableWidget *table, int rowIdx)
{
    tableCover = new QTableWidgetItem();
    tableCover->setData(Qt::DecorationRole, this->cover);
    tableCover->setData(Qt::UserRole, this->id);
    table->setItem(rowIdx, 0, tableCover);

    tableActivies = new QTableWidgetItem(this->lastActivies);
    table->setItem(rowIdx, 1, tableActivies);
}

void QScanlateProject::parseVolumes(QJsonObject raw_data)
{
    foreach (const QJsonValue &volume_val, raw_data)
    {
        QVolume *volume = new QVolume(volume_val.toObject(), this);
        this->volumes.append(volume);
    }
}

int QScanlateProject::getChaptersCount()
{
    int ret_val = 0;
    foreach (QVolume *volume, volumes)
        ret_val += volume->getChaptersCount();
    return ret_val;
}

int QScanlateProject::getCompletedChaptersCount()
{
    int ret_val = 0;
    foreach (QVolume *volume, volumes)
        ret_val += volume->getCompletedChaptersCount();
    return ret_val;
}
