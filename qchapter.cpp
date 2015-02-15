#include "qchapter.h"

QChapter::QChapter(QJsonObject raw_data, QObject *parent) :
    QObject(parent)
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
