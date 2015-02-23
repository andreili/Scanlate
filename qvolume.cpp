#include <QJsonArray>
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
    QJsonArray chapters_obj;
    foreach (QChapter *chapter, this->chapters)
        chapters_obj.append(chapter->serialize());
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
    this->chapters.clear();
    foreach (const QJsonValue &chapter_val, raw_data["chapters"].toObject())
    {
        QChapter *chapter = new QChapter(this);
        chapter->deserialize(chapter_val.toObject());
        this->chapters.append(chapter);
    }
    //if (!loaded cover)
    {
        this->cover = QPixmap::fromImage(QImage(":/images/no-cover.gif"));
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

void QVolume::updateOnTree()
{
    int child_count = treeItem->childCount();
    if (child_count > 0)
        for (int i=0 ; i<child_count ; i++)
            treeItem->removeChild(treeItem->child(i));

    treeItem->setText(0, QObject::tr("Том ") + QString::number(this->number) +
                      ((name.length()) ? " \"" + this->name + "\"" : ""));
    foreach (QChapter *chapter, this->chapters)
        chapter->addToTree(treeItem);
}

void QVolume::removeFromTree()
{
    treeItem->treeWidget()->removeItemWidget(treeItem, 0);
    delete treeItem;
}

QChapter* QVolume::getChapterById(int id)
{
    foreach (QChapter *chapter, this->chapters)
        if (chapter->getId() == id)
            return chapter;
    return NULL;
}

QChapter* QVolume::getChapter(int idx)
{
    return this->chapters.at(idx);
}

int QVolume::getCompletedChaptersCount()
{
    int ret_val = 0;
    foreach (QChapter *chapter, this->chapters)
        if (chapter->isCompleted())
            ret_val++;
    return ret_val;
}

void QVolume::updateChapter(QChapter *chapter)
{
    if (chapter->getId() == -1)
    {
        this->chapters.append(chapter);
    }
    else
    {
    }
}
