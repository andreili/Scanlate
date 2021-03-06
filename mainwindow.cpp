#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadingwindow.h"
#include "projectproperties.h"
#include "volumeproperties.h"
#include <QAction>
#include <QMessageBox>
#include <QJsonDocument>
#include <QColorDialog>
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
    delete this->scanlate;
    delete ui;
}

void MainWindow::setUser(QString login, QString token)
{
    m_logged_as = login;
    m_token = token;
    m_login_label->setText(login);
}

#define LOADING_PROJECTS_STEPS 4

void MainWindow::LoadProjectsList()
{
    emit UpdateProgress(QObject::tr("Сведения о пользователе"), LOADING_PROJECTS_STEPS, 1);
    scanlate->getUserInfo();
    ui->lLogin->setText(this->scanlate->getUser()->getLogin());

    emit UpdateProgress(QObject::tr("Список участников"), LOADING_PROJECTS_STEPS, 2);
    scanlate->getUsersList();

    emit UpdateProgress(QObject::tr("Список проектов"), LOADING_PROJECTS_STEPS, 3);
    scanlate->getProjectsList(ui->twProjects);

    emit UpdateProgress("", LOADING_PROJECTS_STEPS, LOADING_PROJECTS_STEPS + 1);
}

void MainWindow::UpdateProjectInfo(QScanlateProject *project)
{
    if (project->getId() != -1)
        this->scanlate->UpdateProjectInfo(project);
    else
        this->scanlate->addNewProject(project, ui->twProjects);
}

void MainWindow::UpdateVolumeInfo(QVolume *volume)
{
    if (volume->getId() != -1)
    {
        this->scanlate->updateVolumeInfo(volume);
    }
    else
        this->scanlate->addNewVolume(volume, this->ui->twChapters);
}

#define LOADING_CHAPTER_STEPS 4

void MainWindow::LoadChapterFiles()
{
    emit UpdateProgress(QObject::tr("Стили главы"), LOADING_CHAPTER_STEPS, 1);
    scanlate->loadStyles(scanlate->getActiveProject(), ui->lwStyles);

    emit UpdateProgress(QObject::tr("Перевод главы"), LOADING_CHAPTER_STEPS, 2);
    scanlate->loadTranslate(this->activeChapter);

    emit UpdateProgress(QObject::tr("Оригинал главы"), LOADING_CHAPTER_STEPS, 3);
    scanlate->loadRAW(this->activeChapter);

    emit UpdateProgress(QObject::tr("Очищенная глава"), LOADING_CHAPTER_STEPS, 4);
    if (scanlate->getUser()->isTyper())
        scanlate->loadClean(this->activeChapter);

    // TODO
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

void MainWindow::projectPropertiesDialog(QScanlateProject *project)
{
    ProjectProperties properties(project, this->scanlate->getUser()->isModerator(), this);
    connect(&properties, SIGNAL(UpdateProjectInfo(QScanlateProject*)),
            this, SLOT(UpdateProjectInfo(QScanlateProject*)));
    properties.exec();
}

void MainWindow::setActiveProject(QScanlateProject *project)
{
    this->scanlate->setActiveProject(project);
    m_project_label->setText(QObject::tr("Активный проект: ") + project->getName());
    this->scanlate->getChaptersList(project, ui->twChapters);
    ui->twChapters->setEnabled(true);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tab_chapters));
}

void MainWindow::addNewVolumeDialog()
{
    QVolume *volume = new QVolume();
    volume->deserialize(QJsonDocument::fromJson("{\"id\":-1}").object());
    volumePropertiesDialog(volume);
}

void MainWindow::volumePropertiesDialog(QVolume *volume)
{
    VolumeProperties properties(volume, this);
    connect(&properties, SIGNAL(UpdateVolumeInfo(QVolume*)),
            this, SLOT(UpdateVolumeInfo(QVolume*)));
    properties.exec();
}

void MainWindow::setActiveChapter(QChapter *chapter)
{
    this->activeChapter = chapter;
    this->m_chapter_label->setText(QObject::tr("Глава: ") + QString::number(chapter->getNumber()));

    LoadingWindow loadingWindow(this);
    connect(&loadingWindow, SIGNAL(loadingProc()), this, SLOT(LoadChapterFiles()));
    connect(this, SIGNAL(UpdateProgress(QString,int,int)), &loadingWindow, SLOT(UpdateProgress(QString,int,int)));
    loadingWindow.Start();
    loadingWindow.exec();
}

void MainWindow::on_twProjects_doubleClicked(const QModelIndex &index)
{
    int project_id = ui->twProjects->item(index.row(), 0)->data(Qt::UserRole).toInt();  // TODO: SIGSEG для нового проекта
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
    else
        return;

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
        this->scanlate->deleteProject(project);
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
    else
        return;

    if(!action)
        return;
    else if (action->data().toString() == "setActive")
        setActiveChapter(chapter);
    else if ((action->data().toString() == "addChapter") || (action->data().toString() == "addChapterEx"))
        volumePropertiesDialog(volume);
    else if (action->data().toString() == "addVolume")
        addNewVolumeDialog();
    else if (action->data().toString() == "editVolume")
        volumePropertiesDialog(volume);
    else if (action->data().toString() == "delVolume")
        this->scanlate->deleteVolume(volume); // TODO: Internal server error
    else if (action->data().toString() == "delChapter")
        this->scanlate->deleteChapter(chapter); // TODO: Internal server error
}

void MainWindow::saveState()
{
    scanlate->saveState("./data/");
}

void MainWindow::on_MainWindow_destroyed()
{
    scanlate->saveState("./data/");
}

#define updateSampleFont() \
{ \
    QFont font(ui->cbStyleFonts->currentFont().family(), ui->sbStyleSize->value(), \
        (ui->cbBold->checkState() == Qt::Checked ? QFont::Bold : QFont::Normal), \
        ui->cbItalic->checkState() == Qt::Checked); \
    ui->lStyleSample->setFont(font); \
}
// TODO: color

void MainWindow::on_lwStyles_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QTranslateStyle *style = scanlate->getActiveProject()->getStyle(current->text());
    QFont font;
    font.setFamily(style->getFontName());

    ui->leStyleName->setText(style->getName());
    ui->cbStyleFonts->setCurrentFont(font);
    ui->sbStyleSize->setValue(style->getSize());
    ui->cbItalic->setChecked(style->getItalic());
    ui->cbBold->setChecked(style->getBold());
    updateSampleFont();
}

void MainWindow::on_cbStyleFonts_currentFontChanged(const QFont &f)
{
    updateSampleFont();
}

void MainWindow::on_sbStyleSize_valueChanged(int arg1)
{
    updateSampleFont();
}

void MainWindow::on_cbItalic_stateChanged(int arg1)
{
    updateSampleFont();
}

void MainWindow::on_cbBold_stateChanged(int arg1)
{
    updateSampleFont();
}

void MainWindow::on_pbColor_clicked()
{
    /*QColor color = QColorDialog::getColor(Qt::black, this);
    if (color.isValid())
    {
        updateSampleFont();
        QPalette pal(ui->lStyleSample->palette());
        pal.setColor(QPalette::WindowText, color);
        ui->lStyleSample->setPalette(pal);
    }*/
}

void MainWindow::on_pbAdd_clicked()
{
    QTranslateStyle *style = new QTranslateStyle();
    style->setName(ui->leStyleName->text());
    style->setFontName(ui->cbStyleFonts->currentFont().family());
    style->setSize(ui->sbStyleSize->value());
    style->setItalic(ui->cbItalic->checkState() == Qt::Checked);
    style->setBold(ui->cbBold->checkState() == Qt::Checked);
    scanlate->getActiveProject()->addStyle(style, ui->lwStyles);
}

void MainWindow::on_pbSave_clicked()
{
    QTranslateStyle *style = scanlate->getActiveProject()->getStyle(ui->lwStyles->currentItem()->text());
    style->setName(ui->leStyleName->text());
    style->setFontName(ui->cbStyleFonts->currentFont().family());
    style->setSize(ui->sbStyleSize->value());
    style->setItalic(ui->cbItalic->checkState() == Qt::Checked);
    style->setBold(ui->cbBold->checkState() == Qt::Checked);
}
