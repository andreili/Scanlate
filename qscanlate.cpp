#include <QMessageBox>
#include <QJsonDocument>
#include "qscanlate.h"

QScanlate::QScanlate(QScanlateServer *server, QObject *parent) :
    QObject(parent)
{
    this->server = server;
    this->activeProject = NULL;
}

void QScanlate::UpdateUserInfo(QScanlateUser *user)
{
    QJsonObject info_json = server->getUserInfo();
    user->setInfo(info_json);
}

void QScanlate::UpdateUsersList()
{
    //QJsonObject users = server->getUsersList();
    QJsonObject users = QJsonDocument::fromJson("{\"error\":0,\"count\":1,\"users\":{\"1\":{\"id\":1,\"login\":\"andreil\","
                                                "\"reg_date\":\"2015-01-23 09:10:27\",\"roles\":{\"1\":{\"id\":1,\"role\":0}}}}}").object();
    if ((users.empty()) || (users["error"].toInt() != 0) || (!users.contains("users")))
    {
        QMessageBox::critical(0, QObject::tr("Ошибка"),
                              QObject::tr("Невозможно получить список пользователей."));
        return;
    }

    users = users["users"].toObject();
    foreach (const QJsonValue &user_json, users)
    {
        QScanlateUser *user = new QScanlateUser(user_json.toObject(), this);
        this->users.append(user);
    }
}

#define InsertSpaceRow(Title) \
    row_idx = table->rowCount(); \
    table->setRowCount(table->rowCount() + 1); \
    space_item = new QTableWidgetItem(Title); \
    space_item->setTextAlignment(Qt::AlignCenter); \
    space_item->setData(Qt::UserRole, -1); \
    table->setItem(row_idx, 0, space_item); \
    table->setSpan(row_idx, 0, 1, table->columnCount());

#define InsertProjectsByStatus(status) \
    foreach (QScanlateProject *project, this->projects) \
        if (project->getStatus() == status) \
        { \
            row_idx = table->rowCount(); \
            table->setRowCount(row_idx + 1); \
            table->setColumnWidth(0, bannerWidth); \
            table->setRowHeight(row_idx, bannerHeight); \
            project->addToTable(table, row_idx); \
        }

void QScanlate::UpdateProjectsList(QTableWidget *table)
{
    //QJsonObject projects = server->getProjectsList();
    QJsonObject projects = QJsonDocument::fromJson("{\"error\":0,"
                            "\"count\":3,"
                            "\"banner\":{\"width\":160,\"height\":220},"
                            "\"projects\":{"
                            "\"1\":{"
                            "\"id\":1,"
                            "\"name\":\"AMG\","
                            "\"status\":0,"
                            "\"cover\":\"\","
                           "\"author\":\"Fujishima Kousuke\","
                           "\"release_date\":1989,"
                           "\"descr\":\"cfevfefefefe\ncevfndsvibdbhivg\"},"
                            "\"2\":{"
                            "\"id\":2,"
                            "\"name\":\"oO\","
                            "\"status\":1,"
                            "\"cover\":\"\"},"
                            "\"3\":{"
                            "\"id\":3,"
                            "\"name\":\"HBW\","
                            "\"status\":2,"
                            "\"cover\":\"\"}}}").object();

    if ((projects.empty()) || (projects["error"].toInt() != 0) || (!projects.contains("projects")))
    {
        QMessageBox::critical(0, QObject::tr("Ошибка"),
                              QObject::tr("Невозможно получить список проектов."));
        return;
    }
    int bannerWidth = projects["banner"].toObject()["width"].toInt();
    int bannerHeight = projects["banner"].toObject()["height"].toInt();

    projects = projects["projects"].toObject();
    foreach (const QJsonValue &project_val, projects)
    {
        QScanlateProject *project = new QScanlateProject(project_val.toObject(), this);
        this->projects.append(project);
    }

    table->clear();
    table->setColumnCount(2);

    QTableWidgetItem *space_item;
    int row_idx;
    InsertSpaceRow(QObject::tr("Активные проекты"));
    InsertProjectsByStatus(QScanlateProject::ProjectStatus::Active);
    InsertSpaceRow(QObject::tr("Завершенные проекты"));
    InsertProjectsByStatus(QScanlateProject::ProjectStatus::Finished);
    InsertSpaceRow(QObject::tr("Неактивные проекты"));
    InsertProjectsByStatus(QScanlateProject::ProjectStatus::Inactive);
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
    //QJsonObject volumes = server->getChaptersList(project->getId());
    QJsonObject volumes = QJsonDocument::fromJson("{\"error\":0,\"count\":1,"
                                                   "\"volumes\":{\"1\":{\"id\":1,\"num\":\"1\",\"name\":\"111111\",\"cover\":\"\",\"project_id\":\"1\","
                                                   "\"chapters\":{\"1\":{\"id\":1,\"volume_id\":\"1\",\"num\":\"1\",\"name\":\"1\"},"
                                                   "\"2\":{\"id\":2,\"volume_id\":\"1\",\"num\":\"2\",\"name\":\"2\"}}}}}").object();
    if ((volumes.empty()) || (volumes["error"].toInt() != 0) || (!volumes.contains("volumes")))
    {
        QMessageBox::critical(0, QObject::tr("Ошибка"),
                              QObject::tr("Невозможно получить список глав."));
        return;
    }

    volumes_tree->clear();
    project->parseVolumes(volumes["volumes"].toObject(), volumes_tree);
}
