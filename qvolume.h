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
    void updateOnTree();
    void removeFromTree();

    QChapter* getChapter(int idx);
    QChapter* getChapterById(int id);
    QList<QChapter*> getChapters() { return this->chapters; }

    int getChaptersCount() { return this->chapters.count(); }
    int getCompletedChaptersCount();

    int getId() { return this->id; }

    QString getName() { return this->name; }
    void setName(QString new_name) { this->name = new_name; }

    int getNumber() { return this->number; }
    void setNumber(int new_number) { this->number = new_number; }

    QPixmap getCover() { return this->cover; }
    void setCover(QPixmap cover) { this->cover = cover; }

    void updateChapter(QChapter *chapter);

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
