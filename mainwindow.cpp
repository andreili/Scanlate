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

    m_login_label = new QLabel("");
    ui->statusBar->addWidget(m_login_label);
    m_project_label = new QLabel("");
    ui->statusBar->addWidget(m_project_label);

    ui->twChapters->setEnabled(false);

    LoadingWindow loadingWindow(this);
    connect(&loadingWindow, SIGNAL(loadingProc()), this, SLOT(LoadProjectsList()));
    connect(this, SIGNAL(UpdateProgress(QString,int,int)), &loadingWindow, SLOT(UpdateProgress(QString,int,int)));
    loadingWindow.Start();
    loadingWindow.exec();
    //scanlate->UpdateUsersList();
    //scanlate->UpdateProjectsList(ui->twProjects);
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
    scanlate->UpdateUserInfo();
    ui->lLogin->setText(this->scanlate->getUser()->getLogin());

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
        menu.addAction(QObject::tr("&Добавить проект"))->setData("addProject");
    }
    else
    {
        menu.addAction(QObject::tr("&Выбрать активным"))->setData("setActiveProject");
        menu.addSeparator();
        menu.addAction(QObject::tr("&Удалить"))->setData("delProject");
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
    else if (action->data().toString() == "setActiveProject")
    {
        QScanlateProject* project = this->scanlate->getProjectByID(project_id);
        this->scanlate->setActiveProject(project);
        m_project_label->setText(QObject::tr("Активный проект: ") + project->getName());
        this->scanlate->getChaptersList(project, ui->twChapters);
        ui->twChapters->setEnabled(true);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tab_chapters));
    }
    else if (action->data().toString() == "addProject")
    {
        // TODO
    }
    else if (action->data().toString() == "delProject")
    {
        // TODO
    }
}

void MainWindow::on_twChapters_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    int volume_id, chapter_id;

    if (!ui->twChapters->currentItem()->parent())
    {
        // selected root item - volume
        volume_id = ui->twChapters->currentItem()->data(0, Qt::UserRole).toInt();
        chapter_id = -1;
        menu.addAction(QObject::tr("&Добавить главу"))->setData("addChapter");
        menu.addAction(QObject::tr("&Добавить том"))->setData("addVolume");
        menu.addAction(QObject::tr("&Редактировать"))->setData("editVolume");
        menu.addSeparator();
        menu.addAction(QObject::tr("&Удалить"))->setData("delVolume");
    }
    else
    {
        // selected choldren item - chapter
        volume_id = ui->twChapters->currentItem()->parent()->data(0, Qt::UserRole).toInt();
        chapter_id = ui->twChapters->currentItem()->data(0, Qt::UserRole).toInt();
        menu.addAction(QObject::tr("&Добавить главу"))->setData("addChapterEx");
        menu.addAction(QObject::tr("&Редактировать"))->setData("editVolume");
        menu.addSeparator();
        menu.addAction(QObject::tr("&Удалить"))->setData("delChapter");
    }

    QVolume *volume = this->scanlate->getActiveProject()->getVolumeById(volume_id);
    QChapter *chapter = volume->getChapterById(chapter_id);

    QAction* action;
    if (pos != QPoint(0,0))
    {
        // Execute context menu
        action =  menu.exec(ui->twChapters->viewport()->mapToGlobal(pos));
    }

    if(!action)
    {
        return;
    }
    else if (action->data().toString() == "addChapter")
    {
        // TODO
    }
    else if (action->data().toString() == "addVolume")
    {
        // TODO
    }
    else if (action->data().toString() == "editVolume")
    {
        // TODO
    }
    else if (action->data().toString() == "delVolume")
    {
        // TODO
    }
    else if (action->data().toString() == "addChapterEx")
    {
        // TODO
    }
    else if (action->data().toString() == "delChapter")
    {
        // TODO
    }
}

void MainWindow::saveState()
{
    scanlate->saveState("./data/");
}

void MainWindow::on_MainWindow_destroyed()
{
    scanlate->saveState("./data/");
}
