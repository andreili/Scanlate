#ifndef QSCANLATEPROJECT_H
#define QSCANLATEPROJECT_H

#include <QObject>
#include <QJsonObject>
#include <QTableWidget>
#include <QPixmap>
#include <QDate>

class QScanlateProject : public QObject
{
    Q_OBJECT
public:
    explicit QScanlateProject(QJsonObject raw_data, QObject *parent = 0);

    enum ProjectStatus
    {
        Active = 0,
        Inactive = 1,
        Finished = 2
    };

    void addToTable(QTableWidget *table, int rowIdx);

    int getId() { return this->id; }
    QString getName() { return this->name; }
    ProjectStatus getStatus() { return this->status; }
    QString getLastActivies() { return this->lastActivies; }
    QString getAuthor() { return this->author; }
    int getReleaseDate() { return this->releaseDate; }
    QString getDescription() { return this->description; }
    QPixmap getCover() { return this->cover; }

private:
    int id;
    QString name;
    QString lastActivies;
    QPixmap cover;
    ProjectStatus status;
    QString author;
    int releaseDate;
    QString description;

    QTableWidgetItem *tableCover;
    QTableWidgetItem *tableActivies;

signals:

public slots:

};

#endif // QSCANLATEPROJECT_H
