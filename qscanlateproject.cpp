#include "qscanlateproject.h"
#include <QBuffer>
#include <QFile>

QScanlateProject::QScanlateProject(QObject *parent) :
    QObject(parent)
{
}

QJsonObject QScanlateProject::serialize()
{
    QJsonObject ret_val;
    ret_val["id"] = this->id;
    ret_val["name"] = this->name;
    ret_val["activies"] = this->lastActivies;
    ret_val["status"] = this->status;
    ret_val["author"] = this->author;
    ret_val["release_date"] = this->releaseDate;
    ret_val["descr"] = this->description;

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    this->cover.save(&buffer, "PNG");
    QString buffer_str = bytes.toBase64();
    ret_val["cover"] = buffer_str;

    return ret_val;
}

void QScanlateProject::deserialize(QJsonObject raw_data)
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

void QScanlateProject::parseVolumes(QJsonObject raw_data, QTreeWidget *volumes_tree)
{
    foreach (const QJsonValue &volume_val, raw_data)
    {
        QVolume *volume = new QVolume(this);
        volume->deserialize(volume_val.toObject());
        volume->addToTree(volumes_tree);
        this->volumes.append(volume);
    }
}

QJsonObject QScanlateProject::serializeVolumes()
{
    QJsonObject ret_val;
    QJsonObject volumes_obj;
    foreach (QVolume *volume, this->volumes)
        volumes_obj[QString::number(volume->getId())] = volume->serialize();
    ret_val["volumes"] = volumes_obj;
    return ret_val;
}

QVolume* QScanlateProject::getVolumeById(int id)
{
    foreach (QVolume *volume, this->volumes)
        if (volume->getId() == id)
            return volume;
    return NULL;
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
