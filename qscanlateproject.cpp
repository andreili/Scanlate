#include "qscanlateproject.h"

QScanlateProject::QScanlateProject(QJsonObject raw_data, QObject *parent) :
    QObject(parent)
{
    this->name = raw_data["name"].toString();
    //subitems << project["cover"].toString();
}

void QScanlateProject::addToTable(QTableWidget *table)
{
    int rowIdx = table->rowCount();
    table->setRowCount(table->rowCount() + 1);
    table->setRowHeight(rowIdx, 190);
    table->setColumnWidth(1, 150);

    tableCover = new QTableWidgetItem();
    //tableCover->setData(Qt::DecorationRole, QPixmap::fromImage(image));
    table->setItem(0, rowIdx, tableCover);

    tableName = new QTableWidgetItem(name);
    table->setItem(1, rowIdx, tableName);
}
