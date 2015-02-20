#include "qchapter.h"

QChapter::QChapter(QObject *parent) :
    QObject(parent)
{
}

QChapter::~QChapter()
{
    // TODO: delete from tree
    //
}

QJsonObject QChapter::serialize()
{
    QJsonObject ret_val;
    ret_val["id"] = this->id;
    ret_val["name"] = this->name;
    ret_val["num"] = QString::number(this->number);
    return ret_val;
}

void QChapter::deserialize(QJsonObject raw_data)
{
    this->id = raw_data["id"].toInt();
    this->name = raw_data["name"].toString();
    this->number = raw_data["num"].toString().toInt();
}

void QChapter::addToTree(QTreeWidgetItem *parent)
{
    treeItem = new QTreeWidgetItem();
    treeItem->setText(0, QObject::tr("Глава ") + QString::number(this->number) + " \"" + this->name + "\"");
    treeItem->setData(0, Qt::UserRole, this->id);
    parent->addChild(treeItem);
}

void QChapter::removeFromTree()
{
    treeItem->treeWidget()->removeItemWidget(treeItem, 0);
    delete treeItem;
}
