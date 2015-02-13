#include "qvolume.h"

QVolume::QVolume(QJsonObject raw_data, QObject *parent) :
    QObject(parent)
{
    this->id = raw_data["id"].toInt();
    this->coverURL = raw_data["cover"].toString();
    this->name = raw_data["name"].toString();
    this->number = raw_data["num"].toInt();
    foreach (const QJsonValue &chapter_val, raw_data)
    {
        QChapter *chapter = new QChapter(chapter_val.toObject(), this);
        this->chapters.append(chapter);
    }
}

int QVolume::getCompletedChaptersCount()
{
    int ret_val = 0;
    foreach (QChapter *chapter, this->chapters)
        if (chapter->isCompleted())
            ret_val++;
    return ret_val;
}

QChapter* QVolume::getChapter(int idx)
{
    return this->chapters.at(idx);
}
