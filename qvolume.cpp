#include "qvolume.h"

QVolume::QVolume(QJsonObject raw_data, QObject *parent) :
    QObject(parent)
{
    this->id = raw_data["id"].toInt();
    this->coverURL = raw_data["cover"].toString();
    this->name = raw_data["name"].toString();
    this->number = raw_data["num"].toString().toInt();
    foreach (const QJsonValue &chapter_val, raw_data["chapters"].toObject())
    {
        QChapter *chapter = new QChapter(chapter_val.toObject(), this);
        this->chapters.append(chapter);
    }
}

void QVolume::addToTree(QTreeWidget *tree)
{
    treeItem = new QTreeWidgetItem();
    treeItem->setText(0, QObject::tr("Том ") + QString::number(this->number));
    treeItem->setData(0, Qt::UserRole, this->id);
    tree->addTopLevelItem(treeItem);
    foreach (QChapter *chapter, this->chapters)
        chapter->addToTree(treeItem);
}

QChapter* QVolume::getChapterById(int id)
{
    foreach (QChapter *chapter, this->chapters)
        if (chapter->getId() == id)
            return chapter;
    return NULL;
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
