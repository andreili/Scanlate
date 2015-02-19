#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadingwindow.h"
#include "projectproperties.h"
#include <QAction>
#include <QMessageBox>
#include <QJsonDocument>
#include <QStandardItemModel>

MainWindow::MainWindow(QScanlateServer *server, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->scanlate = new QScanlate(server, this);

    m_login_label = new QLabel(this->scanlate->getUser()->getLogin());
    ui->statusBar->addWidget(m_login_label);
    m_network_label = new QLabel((this->scanlate->getMode() == QScanlateServer::NORNAL) ? "online" : "offline");
    ui->statusBar->addWidget(m_network_label);
    m_project_label = new QLabel("");
    ui->statusBar->addWidget(m_project_label);
    m_chapter_label = new QLabel("");
    ui->statusBar->addWidget(m_chapter_label);

    ui->twChapters->setEnabled(false);

    LoadingWindow loadingWindow(this);
    connect(&loadingWindow, SIGNAL(loadingProc()), this, SLOT(LoadProjectsList()));
    connect(this, SIGNAL(UpdateProgress(QString,int,int)), &loadingWindow, SLOT(UpdateProgress(QString,int,int)));
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

#define LOADING_STEPS 4

void MainWindow::LoadProjectsList()
{
    emit UpdateProgress(QObject::tr("Сведения о пользователе"), LOADING_STEPS, 1);
    scanlate->getUserInfo();
    ui->lLogin->setText(this->scanlate->getUser()->getLogin());

    emit UpdateProgress(QObject::tr("Список участников"), LOADING_STEPS, 2);
    scanlate->getUsersList();

    emit UpdateProgress(QObject::tr("Список проектов"), LOADING_STEPS, 3);
    scanlate->getProjectsList(ui->twProjects);

    emit UpdateProgress("", LOADING_STEPS, LOADING_STEPS + 1);
}

void MainWindow::UpdateProjectInfo(QScanlateProject *project)
{
    this->scanlate->UpdateProjectInfo(project);
    if (project->getId() != -1)
    {
        int row = ui->twProjects->currentRow();
        ui->twProjects->item(row, 0)->setData(Qt::DecorationRole, project->getCover());
    }
    else
    {
        this->scanlate->addNewProject(project, ui->twProjects);
    }
}

void MainWindow::setEnabledMenu(const QMenu &menu)
{
    QList<QAction*>actions = menu.actions();
    foreach (QAction* action, actions)
    {
        QString actionName = action->data().toString();
        if ((actionName.startsWith("add")) || (actionName.startsWith("del"))|| (actionName.startsWith("edit")))
        {
            action->setEnabled(this->scanlate->getMode() == QScanlateServer::NORNAL);
        }
    }
}

void MainWindow::addNewProjectDialog()
{
    QScanlateProject *project = new QScanlateProject();
    project->deserialize(QJsonDocument::fromJson("{\"id\":-1}").object());
    projectPropertiesDialog(project);
}

bool MainWindow::projectPropertiesDialog(QScanlateProject *project)
{
    ProjectProperties properties(project, this->scanlate->getUser()->isModerator(), this);
    connect(&properties, SIGNAL(UpdateProjectInfo(QScanlateProject*)),
            this, SLOT(UpdateProjectInfo(QScanlateProject*)));
    properties.show();
    return (properties.result() == QDialog::DialogCode::Accepted);
}

void MainWindow::setActiveProject(QScanlateProject *project)
{
    this->scanlate->setActiveProject(project);
    m_project_label->setText(QObject::tr("Активный проект: ") + project->getName());
    this->scanlate->getChaptersList(project, ui->twChapters);
    ui->twChapters->setEnabled(true);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tab_chapters));
}

void MainWindow::deleteProject(QScanlateProject *project)
{
    // TODO
}

void MainWindow::addNewVolumeDialog()
{
    // TODO
}

void MainWindow::volumePropertiesDialog(QVolume *volume)
{
    // TODO
}

void MainWindow::deleteVolume(QVolume *volume)
{
    // TODO
}

void MainWindow::addNewChapterDialog(QVolume *volume)
{
    // TODO
}

void MainWindow::setActiveChapter(QChapter *chapter)
{
    this->m_chapter_label->setText(QObject::tr("Глава: ") + QString::number(chapter->getNumber()));
    // TODO
}

void MainWindow::deleteChapter(QVolume *volume, QChapter *chapter)
{
    // TODO
}

void MainWindow::on_twProjects_doubleClicked(const QModelIndex &index)
{
    int project_id = ui->twProjects->item(index.row(), 0)->data(Qt::UserRole).toInt();
    if (project_id == -1)
    {
        if (this->scanlate->getMode() == QScanlateServer::NORNAL)
        {
            addNewProjectDialog();
        }
        else
        {
            QMessageBox::critical(this, QObject::tr("Ошибка"),
                                    QObject::tr("Данный функционал доступен только в онлайн-режиме!"));
            return;
        }
    }
    else
        projectPropertiesDialog(scanlate->getProjectByID(project_id));
}

void MainWindow::on_twProjects_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu;
    int project_id = ui->twProjects->item(ui->twProjects->currentRow(), 0)->data(Qt::UserRole).toInt();
    if (project_id == -1)
    {
        menu.addAction(QObject::tr("&Добавить проект"))->setData("addProject");
    }
    else
    {
        menu.addAction(QObject::tr("&Свойства"))->setData("propertiesProject");
        menu.addAction(QObject::tr("&Выбрать активным"))->setData("setActiveProject");
        menu.addSeparator();
        menu.addAction(QObject::tr("&Добавить проект"))->setData("addProject");
        menu.addAction(QObject::tr("&Удалить"))->setData("delProject");
    }
    this->setEnabledMenu(menu);

    QAction* action;
    if (pos != QPoint(0,0))
        action =  menu.exec(ui->twProjects->viewport()->mapToGlobal(pos));

    QScanlateProject *project = this->scanlate->getProjectByID(project_id);
    if(!action)
        return;
    else if (action->data().toString() == "setActiveProject")
        setActiveProject(project);
    else if (action->data().toString() == "propertiesProject")
        projectPropertiesDialog(project);
    else if (action->data().toString() == "addProject")
        addNewProjectDialog();
    else if (action->data().toString() == "delProject")
        deleteProject(project);
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
        // selected children item - chapter
        volume_id = ui->twChapters->currentItem()->parent()->data(0, Qt::UserRole).toInt();
        chapter_id = ui->twChapters->currentItem()->data(0, Qt::UserRole).toInt();

        menu.addAction(QObject::tr("&Выбрать активной"))->setData("setActive");
        menu.addAction(QObject::tr("&Добавить главу"))->setData("addChapterEx");
        menu.addAction(QObject::tr("&Редактировать"))->setData("editVolume");
        menu.addSeparator();
        menu.addAction(QObject::tr("&Удалить"))->setData("delChapter");
    }
    this->setEnabledMenu(menu);

    QVolume *volume = this->scanlate->getActiveProject()->getVolumeById(volume_id);
    QChapter *chapter = volume->getChapterById(chapter_id);

    QAction* action;
    if (pos != QPoint(0,0))
        action =  menu.exec(ui->twChapters->viewport()->mapToGlobal(pos));

    if(!action)
        return;
    else if (action->data().toString() == "setActive")
        setActiveChapter(chapter);
    else if ((action->data().toString() == "addChapter") || (action->data().toString() == "addChapterEx"))
        addNewChapterDialog(volume);
    else if (action->data().toString() == "addVolume")
        addNewVolumeDialog();
    else if (action->data().toString() == "editVolume")
        volumePropertiesDialog(volume);
    else if (action->data().toString() == "delVolume")
        deleteVolume(volume);
    else if (action->data().toString() == "delChapter")
        deleteChapter(volume, chapter);
}

void MainWindow::saveState()
{
    scanlate->saveState("./data/");
}

void MainWindow::on_MainWindow_destroyed()
{
    scanlate->saveState("./data/");
}
