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
    ~QScanlate();

    void saveState(QString directory);

    void getUserInfo();
    void getUsersList();
    void getProjectsList(QTableWidget *table);

    void updateProjectsTable(QTableWidget *table);
    void UpdateProjectInfo(QScanlateProject *project);

    QScanlateServer::NetworkMode getMode() { return this->mode; }

    QScanlateUser* getUser() { return this->user; }

    void getChaptersList(QScanlateProject *project, QTreeWidget *volumes_tree);

    bool isLogged();

    /* Project managment */
    QScanlateProject* getProjectByID(int id);
    // call with project contains ID is a -1
    void addNewProject(QScanlateProject *project, QTableWidget *table);
    void deleteProject(QScanlateProject *project);

    QScanlateProject* getActiveProject() { return this->activeProject; }
    void setActiveProject(QScanlateProject* new_val) { this->activeProject = new_val; }

private:
    QScanlateServer::NetworkMode mode;
    QScanlateServer *server;
    QList<QScanlateUser*> users;
    QList<QScanlateProject*> projects;
    QScanlateProject* activeProject;
    QScanlateUser *user;

    int lastActiveProjectRow;
    int lastInactiveProjectRow;
    int lastFinishedProjectRow;

signals:

public slots:

};

#endif // QSCANLATE_H
