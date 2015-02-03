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

    void UpdateUserInfo();
    void UpdateProjectsList(QTableWidget *table);

    bool isLogged();

    QString getLogin() { return login; }

private:
    QString login;

    QScanlateServer *server;
    QList<QScanlateProject*> projects;

signals:

public slots:

};

#endif // QSCANLATE_H
