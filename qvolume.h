#ifndef QVOLUME_H
#define QVOLUME_H

#include <QObject>
#include <QPixmap>
#include <QJsonObject>
#include <QTreeWidget>
#include "qchapter.h"

class QVolume : public QObject
{
    Q_OBJECT
public:
    explicit QVolume(QObject *parent = 0);
    ~QVolume();

    QJsonObject serialize();
    void deserialize(QJsonObject raw_data);

    void addToTree(QTreeWidget *tree);

    QChapter* getChapterById(int id);

    int getChaptersCount() { return this->chapters.count(); }
    int getCompletedChaptersCount();

    int getId() { return this->id; }

    QString getName() { return this->name; }

    int getNumber() { return this->number; }

    QPixmap getCover() { return this->cover; }

    QChapter* getChapter(int idx);

private:
    QList<QChapter*> chapters;
    int id;
    QPixmap cover;
    QString coverURL;
    QString name;
    int number;

    QTreeWidgetItem *treeItem;

signals:

public slots:

};

#endif // QVOLUME_H
