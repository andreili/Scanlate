#ifndef QSCANLATEPROJECT_H
#define QSCANLATEPROJECT_H

#include <QObject>
#include <QJsonObject>
#include <QTableWidget>
#include <QPixmap>

class QScanlateProject : public QObject
{
    Q_OBJECT
public:
    explicit QScanlateProject(QJsonObject raw_data, QObject *parent = 0);

    enum ProjectStatus
    {
        Active,
        Inactive,
        Finished
    };

    void addToTable(QTableWidget *table);

    ProjectStatus getStatus() { return ProjectStatus::Active; }

private:
    QString name;
    QPixmap cover;

    QTableWidgetItem *tableCover;
    QTableWidgetItem *tableName;

signals:

public slots:

};

#endif // QSCANLATEPROJECT_H
