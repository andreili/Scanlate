#include "qscanlateproject.h"

QScanlateProject::QScanlateProject(QJsonObject raw_data, QObject *parent) :
    QObject(parent)
{
    this->name = raw_data["name"].toString();
    //subitems << project["cover"].toString();
    //QImage image()
    //this->cover = QPixmap::fromImage()
    this->cover->loadFromData(QByteArray::fromBase64(raw_data["cover"].toString()), "PNG");
}

void QScanlateProject::addToTable(QTableWidget *table)
{
    int rowIdx = table->rowCount();
    table->setRowCount(table->rowCount() + 1);
    //table->setRowHeight(rowIdx, 190);
    //table->setColumnWidth(1, 150);

    tableCover = new QTableWidgetItem();
    //tableCover->setData(Qt::DecorationRole, QPixmap::fromImage(image));
    table->setItem(rowIdx, 0, tableCover);

    tableName = new QTableWidgetItem(name);
    table->setItem(rowIdx, 1, tableName);
}
