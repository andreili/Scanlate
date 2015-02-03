#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadingwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include "api.h"

MainWindow::MainWindow(QString server, QString login, QString token, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_server = server;
    m_logged_as = login;
    m_token = token;
    m_login_label = new QLabel(login);
    ui->statusBar->addWidget(m_login_label);

    m_active_projects_root = new QTreeWidgetItem(QStringList(QObject::tr("Активные проекты")));
    m_finished_projects_root = new QTreeWidgetItem(QStringList(QObject::tr("Заверешнные проекты")));
    m_inactive_projects_root = new QTreeWidgetItem(QStringList(QObject::tr("Неактивные проекты")));
    ui->twProjects->addTopLevelItem(m_active_projects_root);
    ui->twProjects->addTopLevelItem(m_finished_projects_root);
    ui->twProjects->addTopLevelItem(m_inactive_projects_root);

    ui->twProjects->setColumnCount(3);
    ui->twProjects->setHeaderHidden(false);
    QStringList headers;
    headers << QObject::tr("Обложка");
    headers << QObject::tr("Название");
    headers << QObject::tr("Активных участников");
    ui->twProjects->setHeaderLabels(headers);

    LoadingWindow loadingWindow(QObject::tr("Список проектов"), this);
    connect(&loadingWindow, SIGNAL(loadingProc()), this, SLOT(LoadProjectsList()));
    connect(this, SIGNAL(UpdateProgress(int,int)), &loadingWindow, SLOT(UpdateProgress(int,int)));
    loadingWindow.Start();
    loadingWindow.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUser(QString login, QString token)
{
    m_logged_as = login;
    m_token = token;
    m_login_label->setText(login);
}

void MainWindow::LoadProjectsList()
{
    emit UpdateProgress(0, 0);

    QUrl url(m_server + "/data.php");
    url.setQuery("query=projects_list");
    QJsonObject json_reply = api.query(url);

    if (!json_reply.empty())
    {
        if ((json_reply["error"].toInt() == 0) && (json_reply.contains("projects")))
        {
            QJsonObject projects = json_reply["projects"].toObject();
            QList<QString> items = projects.keys();

            // active projects
            foreach (const QJsonValue &project_val, projects)
            {
                QJsonObject project = project_val.toObject();
                if (project["status"].toInt() == 0)
                {
                    QStringList subitems;
                    subitems << project["cover"].toString();
                    subitems << project["name"].toString();
                    subitems << "-";
                    QTreeWidgetItem *item = new QTreeWidgetItem(subitems);
                    m_active_projects_root->addChild(item);
                }
            }
        }
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Невозможно получить список проектов!"));
    }
    ui->twProjects->expandAll();
    emit UpdateProgress(0, 1);
}
