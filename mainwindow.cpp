#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadingwindow.h"
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

    /*LoadingWindow loadingWindow(this);
    connect(&loadingWindow, SIGNAL(loadingProc()), this, SLOT(LoadProjectsList()));
    connect(this, SIGNAL(UpdateProgress(QString,int,int)), &loadingWindow, SLOT(UpdateProgress(QString,int,int)));
    loadingWindow.Start();
    loadingWindow.exec();*/
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
    scanlate->UpdateUserInfo();
    ui->lLogin->setText(scanlate->getLogin());

    emit UpdateProgress(QObject::tr("Список проектов"), LOADING_STEPS, 2);
    scanlate->UpdateProjectsList(ui->twProjects);

    emit UpdateProgress("", LOADING_STEPS, LOADING_STEPS + 1);
}
