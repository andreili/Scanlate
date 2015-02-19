#ifndef QSCANLATEPROJECT_H
#define QSCANLATEPROJECT_H

#include <QObject>
#include <QJsonObject>
#include <QTableWidget>
#include <QTreeWidget>
#include <QPixmap>
#include <QDate>
#include "qvolume.h"

class QScanlateProject : public QObject
{
    Q_OBJECT
public:
    explicit QScanlateProject(QObject *parent = 0);

    QJsonObject serialize();
    void deserialize(QJsonObject raw_data);

    enum ProjectStatus
    {
        Active = 0,
        Inactive = 1,
        Finished = 2
    };

    void addToTable(QTableWidget *table, int rowIdx);
    void updateOnTable();

    QVolume* getVolumeById(int id);

    int getId() { return this->id; }
    QString getName() { return this->name; }
    void setName(QString name) { this->name = name; }

    ProjectStatus getStatus() { return this->status; }
    void setStatus(ProjectStatus status) { this->status = status; }

    QString getAuthor() { return this->author; }
    void setAuthor(QString author) { this->author = author; }

    int getReleaseDate() { return this->releaseDate; }
    void setReleaseDate(int releaseDate) { this->releaseDate = releaseDate; }

    QString getDescription() { return this->description; }
    void setDescription(QString description) { this->description = description; }

    QPixmap getCover() { return this->cover; }
    void setCover(QPixmap cover) { this->cover = cover; }

    void parseVolumes(QJsonObject raw_data, QTreeWidget *volumes_tree);
    QJsonObject serializeVolumes();

    int getVolumesCount() { return this->volumes.count(); }

    int getChaptersCount();
    int getCompletedChaptersCount();

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

    QList<QVolume*> volumes;

signals:

public slots:

};

#endif // QSCANLATEPROJECT_H
