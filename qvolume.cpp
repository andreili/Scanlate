#include "qvolume.h"

QVolume::QVolume(QObject *parent) :
    QObject(parent)
{
}

QJsonObject QVolume::serialize()
{
    QJsonObject ret_val;
    ret_val["id"] = this->id;
    ret_val["cover"] = this->coverURL;  // TODO
    ret_val["name"] = this->name;
    ret_val["num"] = this->number;
    QJsonObject chapters_obj;
    foreach (QChapter *chapter, this->chapters)
        chapters_obj[QString::number(chapter->getId())] = chapter->serialize();
    ret_val["chapters"] = chapters_obj;

    return ret_val;
}

QVolume::~QVolume()
{
    // TODO: delete from tree
    //
    foreach (QChapter *chapter, this->chapters)
        delete chapter;
}

void QVolume::deserialize(QJsonObject raw_data)
{
    this->id = raw_data["id"].toInt();
    this->coverURL = raw_data["cover"].toString();
    this->name = raw_data["name"].toString();
    this->number = raw_data["num"].toString().toInt();
    foreach (const QJsonValue &chapter_val, raw_data["chapters"].toObject())
    {
        QChapter *chapter = new QChapter(this);
        chapter->deserialize(chapter_val.toObject());
        this->chapters.append(chapter);
    }
}

void QVolume::addToTree(QTreeWidget *tree)
{
    treeItem = new QTreeWidgetItem();
    treeItem->setText(0, QObject::tr("Том ") + QString::number(this->number) +
                      ((name.length()) ? " \"" + this->name + "\"" : ""));
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
