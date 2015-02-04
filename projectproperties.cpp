#include <QFileDialog>
#include "projectproperties.h"
#include "ui_projectproperties.h"

ProjectProperties::ProjectProperties(QScanlateProject *project, bool editable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectProperties)
{
    ui->setupUi(this);

    ui->cbReleaseDate->clear();
    for (int i=1970 ; i<2015 ; i++)
        ui->cbReleaseDate->addItem(QString::number(i));

    ui->leName->setText(project->getName());
    ui->leAuthor->setText(project->getAuthor());
    ui->cbReleaseDate->setCurrentIndex(project->getReleaseDate() - 1970);
    ui->teDescription->setText(project->getDescription());
    ui->lCover->setPixmap(project->getCover());
    switch (project->getStatus())
    {
    case QScanlateProject::Active:
        ui->cbStatus->setCurrentIndex(0);
        break;
    case QScanlateProject::Inactive:
        ui->cbStatus->setCurrentIndex(1);
        break;
    case QScanlateProject::Finished:
        ui->cbStatus->setCurrentIndex(2);
        break;
    }
    if (!editable)
    {
        ui->leName->setEnabled(false);
        ui->leAuthor->setEnabled(false);
        ui->cbReleaseDate->setEnabled(false);
        ui->teDescription->setEnabled(false);
        ui->pbChangeCover->setEnabled(false);
        ui->cbStatus->setEnabled(false);
    }
}

ProjectProperties::~ProjectProperties()
{
    delete ui;
}

void ProjectProperties::on_pbChangeCover_pressed()
{
}

void ProjectProperties::on_pbChangeCover_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Укажите файл обложки"), "",
                                                    QObject::tr("Файлы изображений (*.gif *.jpg *.png)"));
    if (fileName.isEmpty())
    {
        ui->lCover->setPixmap(QPixmap::fromImage(QImage(fileName)));
    }
}
