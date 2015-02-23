#ifndef QSCANLATE_H
#define QSCANLATE_H

#include <QObject>
#include <QTableWidget>
#include <QListWidget>
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

    QScanlateServer::NetworkMode getMode() { return this->mode; }

    QScanlateUser* getUser() { return this->user; }

    bool isLogged();

    /* Project managment */
    QScanlateProject* getProjectByID(int id);
    // call with project contains ID is a -1
    void addNewProject(QScanlateProject *project, QTableWidget *table);
    void UpdateProjectInfo(QScanlateProject *project);
    void deleteProject(QScanlateProject *project);

    void addNewVolume(QVolume *volume, QTreeWidget *volumes_tree);
    void updateVolumeInfo(QVolume *volume);
    void deleteVolume(QVolume *volume);
    void deleteChapter(QChapter *chapter);

    QScanlateProject* getActiveProject() { return this->activeProject; }
    void setActiveProject(QScanlateProject* new_val) { this->activeProject = new_val; }

    void getChaptersList(QScanlateProject *project, QTreeWidget *volumes_tree);
    void loadStyles(QScanlateProject *project, QListWidget *list);
    void loadTranslate(QChapter *chapter);
    void loadRAW(QChapter *chapter);
    void loadClean(QChapter *chapter);

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

    QString getProjectFolder();

signals:

public slots:

};

#endif // QSCANLATE_H
