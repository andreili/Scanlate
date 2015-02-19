#include <QMessageBox>
#include <QJsonDocument>
#include <QDir>
#include "qscanlate.h"

#define COVER_WIDTH 160
#define COVER_HEIGHT 220

QScanlate::QScanlate(QScanlateServer *server, QObject *parent) :
    QObject(parent)
{
    this->server = server;
    this->activeProject = NULL;
    this->user = new QScanlateUser();
    this->mode = this->server->getMode();
}

QScanlate::~QScanlate()
{
    delete this->server;
    foreach (QScanlateUser *user, this->users)
        delete user;
    delete this->user;
    foreach (QScanlateProject *project, this->projects)
        delete project;
}

#define save_json(file_name, object) \
{ \
    data = QJsonDocument(object).toJson(); \
    QFile file(file_name); \
    file.open(QIODevice::WriteOnly); \
    file.write(data); \
    file.close(); \
}

#define save_list(prefix, file_name, obj_type, sub_name) \
    if (!this->prefix ## s.empty()) \
    { \
        QJsonObject prefix ## s; \
        QJsonObject prefix ## s_obj; \
        foreach (obj_type *prefix, this->prefix ## s) \
            prefix ## s_obj[QString::number(prefix->getId())] = prefix->serialize(); \
        prefix ## s[sub_name] = prefix ## s_obj; \
        save_json(directory + file_name, prefix ## s); \
    }

void QScanlate::saveState(QString directory)
{
    if (!QDir(directory).exists())
        QDir().mkpath(directory);

    QByteArray data;

    save_list(user, "users.json", QScanlateUser, "users");
    save_list(project, "projects.json", QScanlateProject, "projects");

    if (this->activeProject)
    {
        QJsonObject volumes = this->activeProject->serializeVolumes();
        save_json(directory + "project_" + QString::number(this->activeProject->getId()) + ".json",
                  volumes);
    }

    QJsonObject user = this->user->serialize();
    user["token"] = this->server->getToken();
    data = QJsonDocument(user).toJson();
    QFile file_user(directory + "user.json");
    file_user.open(QIODevice::WriteOnly);
    file_user.write(data);
    file_user.close();
}

#define read_json(file_name, ret_var) \
{ \
    QFile file(file_name); \
    file.open(QIODevice::ReadOnly); \
    QByteArray data = file.readAll(); \
    ret_var = QJsonDocument::fromJson(data).object(); \
}

void QScanlate::getUserInfo()
{
    QJsonObject info_json;
    if (this->mode == QScanlateServer::NORNAL)
        info_json = server->getUserInfo();
    else
    {
        read_json("./data/user.json", info_json);
        this->server->setToken(info_json["token"].toString());
    }
    this->user->deserialize(info_json);
}

void QScanlate::getUsersList()
{
    QJsonObject users;
    if (this->mode == QScanlateServer::NORNAL)
        users = server->getUsersList();
    else
        read_json("./data/users.json", users);

    if ((users.empty())
            || (users["error"].toInt() != 0)
            || (!users.contains("users")))
    {
        QMessageBox::critical(0, QObject::tr("Ошибка"),
                              QObject::tr("Невозможно получить список пользователей."));
        return;
    }

    users = users["users"].toObject();
    foreach (const QJsonValue &user_json, users)
    {
        QScanlateUser *user = new QScanlateUser(this);
        user->deserialize(user_json.toObject());
        this->users.append(user);
    }
}

#define InsertSpaceRow(Title) \
{ \
    row_idx = table->rowCount(); \
    table->setRowCount(table->rowCount() + 1); \
    space_item = new QTableWidgetItem(Title); \
    space_item->setTextAlignment(Qt::AlignCenter); \
    space_item->setData(Qt::UserRole, -1); \
    table->setItem(row_idx, 0, space_item); \
    table->setSpan(row_idx, 0, 1, table->columnCount()); \
}

#define InsertProjectToRow(project, row) \
{ \
    table->setColumnWidth(0, COVER_WIDTH); \
    table->setRowHeight(row, COVER_HEIGHT); \
    project->addToTable(table, row); \
}

#define InsertProjectsByStatus(status) \
    foreach (QScanlateProject *project, this->projects) \
        if (project->getStatus() == status) \
        { \
            row_idx = table->rowCount(); \
            table->setRowCount(row_idx + 1); \
            InsertProjectToRow(project, row_idx); \
        }

void QScanlate::getProjectsList(QTableWidget *table)
{
    QJsonObject projects;
    if (this->mode == QScanlateServer::NORNAL)
        projects = server->getProjectsList();
    else
        read_json("./data/projects.json", projects);

    if ((projects.empty())
            || (projects["error"].toInt() != 0)
            || (!projects.contains("projects")))
    {
        QMessageBox::critical(0, QObject::tr("Ошибка"),
                              QObject::tr("Невозможно получить список проектов."));
        return;
    }

    projects = projects["projects"].toObject();
    foreach (const QJsonValue &project_val, projects)
    {
        QScanlateProject *project = new QScanlateProject(this);
        project->deserialize(project_val.toObject());
        this->projects.append(project);
    }

    updateProjectsTable(table);
}

void QScanlate::updateProjectsTable(QTableWidget *table)
{
    table->clear();
    table->setColumnCount(2);

    QTableWidgetItem *space_item;
    int row_idx;
    InsertSpaceRow(QObject::tr("Активные проекты"));
    InsertProjectsByStatus(QScanlateProject::ProjectStatus::Active);
    this->lastActiveProjectRow = row_idx;
    InsertSpaceRow(QObject::tr("Завершенные проекты"));
    InsertProjectsByStatus(QScanlateProject::ProjectStatus::Finished);
    this->lastFinishedProjectRow = row_idx;
    InsertSpaceRow(QObject::tr("Неактивные проекты"));
    InsertProjectsByStatus(QScanlateProject::ProjectStatus::Inactive);
    this->lastInactiveProjectRow = row_idx;
}

void QScanlate::UpdateProjectInfo(QScanlateProject *project)
{
    if (this->mode == QScanlateServer::NORNAL)
    {
        QJsonObject project_info = this->server->UpdateProjectInfo(project->getId(), project->serialize());

        if ((project_info.empty()) || (project_info["error"].toInt() != 0))
        {
            QMessageBox::critical(0, QObject::tr("Ошибка"),
                                  QObject::tr("Невозможно обновить информацию о проекте на сервере!"));
            return;
        }

        project->deserialize(project_info);
        project->updateOnTable();
    }
}

void QScanlate::addNewProject(QScanlateProject *project, QTableWidget *table)
{
    if (this->mode == QScanlateServer::NORNAL)
    {
        QJsonObject project_info = this->server->addNewProject(project->serialize());

        if ((project_info.empty()) || (project_info["error"].toInt() != 0))
        {
            QMessageBox::critical(0, QObject::tr("Ошибка"),
                                  QObject::tr("Невозможно добавить новый проект на сервер!"));
            return;
        }

        project->deserialize(project_info);
        this->projects.append(project);

        switch (project->getStatus())
        {
        case QScanlateProject::ProjectStatus::Active:
            table->insertRow(this->lastActiveProjectRow);
            InsertProjectToRow(project, this->lastActiveProjectRow++);
            this->lastFinishedProjectRow++;
            this->lastInactiveProjectRow++;
            break;
        case QScanlateProject::ProjectStatus::Finished:
            table->insertRow(this->lastFinishedProjectRow);
            InsertProjectToRow(project, this->lastFinishedProjectRow++);
            this->lastInactiveProjectRow++;
            break;
        case QScanlateProject::ProjectStatus::Inactive:
            table->insertRow(this->lastInactiveProjectRow);
            InsertProjectToRow(project, this->lastInactiveProjectRow++);
            break;
        }
    }
}

void QScanlate::deleteProject(QScanlateProject *project)
{
    if (this->mode == QScanlateServer::NORNAL)
    {
        QJsonObject del_result = this->server->deleteProject(project->getId());

        if ((del_result.empty()) || (del_result["error"].toInt() != 0))
        {
            QMessageBox::critical(0, QObject::tr("Ошибка"),
                                  QObject::tr("Невозможно удалить проект на сервере!"));
            return;
        }

        this->projects.removeOne(project);
        delete project;
    }
}

QScanlateProject *QScanlate::getProjectByID(int id)
{
    foreach (QScanlateProject *project, this->projects)
        if (project->getId() == id)
            return project;
    return NULL;
}

void QScanlate::getChaptersList(QScanlateProject *project, QTreeWidget *volumes_tree)
{
    QJsonObject volumes;
    if (this->mode == QScanlateServer::NORNAL)
        volumes = server->getChaptersList(project->getId());
    else
        read_json("./data/project_" + QString::number(this->activeProject->getId()) + ".json", volumes);

    if ((volumes.empty()) || (volumes["error"].toInt() != 0) || (!volumes.contains("volumes")))
    {
        QMessageBox::critical(0, QObject::tr("Ошибка"),
                              QObject::tr("Невозможно получить список глав."));
        return;
    }

    volumes_tree->clear();
    project->parseVolumes(volumes["volumes"].toObject(), volumes_tree);
}
