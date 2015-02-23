#ifndef QCHAPTER_H
#define QCHAPTER_H

#include <QObject>
#include <QJsonObject>
#include <QTreeWidget>
#include "qtranslate.h"

class QChapter : public QObject
{
    Q_OBJECT

    enum ChapterStatus
    {
        Inactive = 0,
        OnWork = 1,
        Finished = 2
    };
public:
    explicit QChapter(QObject *parent = 0);
    ~QChapter();

    QJsonObject serialize();
    void deserialize(QJsonObject raw_data);

    void addToTree(QTreeWidgetItem *parent);
    void removeFromTree();

    bool isCompleted() { return this->status == Finished; }

    int getId() { return this->id; }

    QString getName() { return this->name; }
    void setName(QString new_name) { this->name = new_name; }

    int getNumber() { return this->number; }
    void setNumber(int new_number) { this->number = new_number; }

    QTranslate* getTranslate() { return &this->translate; }

private:
    int id;
    QString name;
    int number;
    ChapterStatus status;
    QTranslate translate;

    QTreeWidgetItem *treeItem;

signals:

public slots:

};

#endif // QCHAPTER_H
