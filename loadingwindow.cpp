#include "loadingwindow.h"
#include "ui_loadingwindow.h"
#include <QDesktopWidget>
#include <QMovie>

LoadingWindow::LoadingWindow(QString obj, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingWindow)
{
    ui->setupUi(this);

    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth = desktop->width();
    int screenHeight = desktop->height();
    int x = (screenWidth - width()) / 2;
    int y = (screenHeight - height()) / 2;
    move( x, y );

    QMovie *movie = new QMovie("./loading.gif");
    ui->lMovie->setMovie(movie);
    movie->start();
    ui->lMovie->setMaximumWidth(movie->frameRect().width());

    ui->lObject->setText(obj);
}

LoadingWindow::~LoadingWindow()
{
    delete ui;
}

void LoadingWindow::Start()
{
    emit loadingProc();
}

void LoadingWindow::UpdateProgress(int all, int completed)
{
    ui->lCounter->setText(QString::number(completed) + "/" + QString::number(all));
    if (completed > all)
        this->close();
}
