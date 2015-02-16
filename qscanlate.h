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

    void saveState(QString directory);

    void UpdateUserInfo();
    void UpdateUsersList();
    void UpdateProjectsList(QTableWidget *table);

    QScanlateUser* getUser() { return this->user; }

    void getChaptersList(QScanlateProject *project, QTreeWidget *volumes_tree);

    bool isLogged();

    QScanlateProject* getProjectByID(int id);

    QScanlateProject* getActiveProject() { return this->activeProject; }
    void setActiveProject(QScanlateProject* new_val) { this->activeProject = new_val; }

private:
    QScanlateServer::NetworkMode mode;
    QScanlateServer *server;
    QList<QScanlateUser*> users;
    QList<QScanlateProject*> projects;
    QScanlateProject* activeProject;
    QScanlateUser *user;

signals:

public slots:

};

#endif // QSCANLATE_H
