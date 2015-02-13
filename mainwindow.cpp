#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadingwindow.h"
#include "projectproperties.h"
#include <QAction>
#include <QMessageBox>
#include <QStandardItemModel>

MainWindow::MainWindow(QScanlateServer *server, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->scanlate = new QScanlate(server, this);
    this->user = new QScanlateUser();

    m_login_label = new QLabel("");
    ui->statusBar->addWidget(m_login_label);
    m_project_label = new QLabel("");
    ui->statusBar->addWidget(m_project_label);

    /*LoadingWindow loadingWindow(this);
    connect(&loadingWindow, SIGNAL(loadingProc()), this, SLOT(LoadProjectsList()));
    connect(this, SIGNAL(UpdateProgress(QString,int,int)), &loadingWindow, SLOT(UpdateProgress(QString,int,int)));
    loadingWindow.Start();
    loadingWindow.exec();*/
    scanlate->UpdateUsersList();
    scanlate->UpdateProjectsList(ui->twProjects);
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

#define LOADING_STEPS 4

void MainWindow::LoadProjectsList()
{
    emit UpdateProgress(QObject::tr("Сведения о пользователе"), LOADING_STEPS, 1);
    scanlate->UpdateUserInfo(this->user);
    ui->lLogin->setText(this->user->getLogin());

    emit UpdateProgress(QObject::tr("Список участников"), LOADING_STEPS, 2);
    scanlate->UpdateUsersList();

    emit UpdateProgress(QObject::tr("Список проектов"), LOADING_STEPS, 3);
    scanlate->UpdateProjectsList(ui->twProjects);

    emit UpdateProgress("", LOADING_STEPS, LOADING_STEPS + 1);
}

void MainWindow::UpdateProjectInfo(int projectID)
{
    if (projectID != -1)
    {
        int row = ui->twProjects->currentRow();
        ui->twProjects->item(row, 0)->setData(Qt::DecorationRole, scanlate->getProjectByID(projectID)->getCover());
    }
    else
    {
        // adding a new project
    }
}

void MainWindow::on_twProjects_doubleClicked(const QModelIndex &index)
{
    int project_id = ui->twProjects->item(index.row(), 0)->data(Qt::UserRole).toInt();
    if (project_id == -1)
    {
        // add new project
    }
    else
    {
        QScanlateProject *project = scanlate->getProjectByID(project_id);
        //ProjectProperties properties(project, this->user->isModerator(), this);
        ProjectProperties properties(project, true, this);
        connect(&properties, SIGNAL(UpdateProjectInfo(int)), this, SLOT(UpdateProjectInfo(int)));
        properties.exec();
    }
}

void MainWindow::on_twProjects_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    int project_id = ui->twProjects->item(ui->twProjects->currentRow(), 0)->data(Qt::UserRole).toInt();;
    if (project_id == -1)
    {
        // add new item
    }
    else
    {
        menu.addAction(QObject::tr("&Выбрать активным"))->setData("active");
    }

    QAction* action;
    if (pos != QPoint(0,0))
    {
        // Execute context menu
        action =  menu.exec(ui->twProjects->viewport()->mapToGlobal(pos));
    }
    if(!action)
    {
        return;
    }
    else if (action->data().toString() == "active")
    {
        // set active project
        QScanlateProject* project = this->scanlate->getProjectByID(project_id);
        this->scanlate->setActiveProject(project);
        m_project_label->setText(QObject::tr("Активный проект: ") + project->getName());
        this->scanlate->getChaptersList(project);
    }
}
