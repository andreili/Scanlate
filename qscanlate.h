#ifndef QSCANLATE_H
#define QSCANLATE_H

#include <QObject>
#include <QTableWidget>
#include "qscanlateserver.h"
#include "qscanlateproject.h"
#include "qscanlateuser.h"

class QScanlate : public QObject
{
    Q_OBJECT
public:
    explicit QScanlate(QScanlateServer *server, QObject *parent = 0);

    void UpdateUserInfo(QScanlateUser *user);
    void UpdateUsersList();
    void UpdateProjectsList(QTableWidget *table);

    bool isLogged();

    QScanlateProject* getProjectByID(int id);

private:
    QScanlateServer *server;
    QList<QScanlateUser*> users;
    QList<QScanlateProject*> projects;

signals:

public slots:

};

#endif // QSCANLATE_H
