#ifndef QCHAPTER_H
#define QCHAPTER_H

#include <QObject>
#include <QJsonObject>
#include <QTreeWidget>

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
    explicit QChapter(QJsonObject raw_data, QObject *parent = 0);

    void addToTree(QTreeWidgetItem *parent);

    bool isCompleted() { return this->status == Finished; }

    int getId() { return this->id; }

    QString getName() { return this->name; }

    int getNumber() { return this->number; }

private:
    int id;
    QString name;
    int number;
    ChapterStatus status;

    QTreeWidgetItem *treeItem;

signals:

public slots:

};

#endif // QCHAPTER_H
