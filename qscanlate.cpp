#include <QMessageBox>
#include <QJsonDocument>
#include "qscanlate.h"

QScanlate::QScanlate(QScanlateServer *server, QObject *parent) :
    QObject(parent)
{
    this->server = server;
}

void QScanlate::UpdateUserInfo()
{
    QJsonObject info_json = server->getUserInfo();
    login = info_json["login"].toString();
}

#define InsertSpaceRow(Title) \
    row_idx = table->rowCount(); \
    table->setRowCount(table->rowCount() + 1); \
    space_item = new QTableWidgetItem(Title); \
    space_item->setTextAlignment(Qt::AlignCenter); \
    space_item->setData(Qt::UserRole, -1); \
    table->setItem(row_idx, 0, space_item); \
    table->setSpan(row_idx, 0, 1, table->columnCount());

void QScanlate::UpdateProjectsList(QTableWidget *table)
{
    //QJsonObject projects = server->getProjectsList();
    QJsonObject projects = QJsonDocument::fromJson("{\"error\":0,"
                            "\"count\":3,"
                            "\"projects\":{"
                            "\"1\":{"
                            "\"id\":\"1\","
                            "\"name\":\"AMG\","
                            "\"status\":\"0\","
                            "\"cover\":\"\"},"
                            "\"2\":{"
                            "\"id\":\"2\","
                            "\"name\":\"oO\","
                            "\"status\":\"1\","
                            "\"cover\":\"\"},"
                            "\"3\":{"
                            "\"id\":\"3\","
                            "\"name\":\"HBW\","
                            "\"status\":\"0\","
                            "\"cover\":\"\"}}}").object();

    if ((projects.empty()) || (projects["error"].toInt() != 0) || (!projects.contains("projects")))
    {
        QMessageBox::critical(0, QObject::tr("Ошибка"),
                              QObject::tr("Невозможно получить список проектов."));
        return;
    }
    projects = projects["projects"].toObject();
    foreach (const QJsonValue &project_val, projects)
    {
        QScanlateProject *project = new QScanlateProject(project_val.toObject(), this);
        this->projects.append(project);
    }

    table->clear();
    table->setColumnCount(3);

    QTableWidgetItem *space_item;
    int row_idx;
    InsertSpaceRow(QObject::tr("Активные проекты"));
    foreach (QScanlateProject *project, this->projects)
        if (project->getStatus() == QScanlateProject::ProjectStatus::Active)
            project->addToTable(table);

    InsertSpaceRow(QObject::tr("Завершенные проекты"));
    foreach (QScanlateProject *project, this->projects)
        if (project->getStatus() == QScanlateProject::ProjectStatus::Finished)
            project->addToTable(table);


    InsertSpaceRow(QObject::tr("Неактивные проекты"));
    foreach (QScanlateProject *project, this->projects)
        if (project->getStatus() == QScanlateProject::ProjectStatus::Inactive)
            project->addToTable(table);
}
