#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadingwindow.h"
#include "projectproperties.h"
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
        properties.exec();
    }
}
