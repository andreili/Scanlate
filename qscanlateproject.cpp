#include "qscanlateproject.h"
#include <QFile>

QScanlateProject::QScanlateProject(QJsonObject raw_data, QObject *parent) :
    QObject(parent)
{
    this->name = raw_data["name"].toString();
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

void QScanlateProject::addToTable(QTableWidget *table)
{
    int rowIdx = table->rowCount();
    table->setRowCount(table->rowCount() + 1);
    table->setRowHeight(rowIdx, 220);
    table->setColumnWidth(0, 160);

    tableCover = new QTableWidgetItem();
    tableCover->setData(Qt::DecorationRole, this->cover);
    table->setItem(rowIdx, 0, tableCover);

    tableName = new QTableWidgetItem(name);
    table->setItem(rowIdx, 1, tableName);
}
