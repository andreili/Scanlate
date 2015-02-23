#include <QJsonDocument>
#include "volumeproperties.h"
#include "ui_volumeproperties.h"

VolumeProperties::VolumeProperties(QVolume *volume, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VolumeProperties)
{
    ui->setupUi(this);

    this->volume = volume;

    ui->sbVolumeNum->setValue(volume->getNumber());
    ui->leVolumeName->setText(volume->getName());
    ui->lCover->setPixmap(volume->getCover());

    for (int i=0 ; i<volume->getChaptersCount() ; i++)
    {
        QChapter *chapter = volume->getChapter(i);
        QListWidgetItem *item = new QListWidgetItem();
        item->setText("[" + QString::number(chapter->getNumber()) + "] " + chapter->getName());
        ui->lwChapters->addItem(item);
        chapters[item] = chapter;
    }
}

VolumeProperties::~VolumeProperties()
{
    delete ui;
}

void VolumeProperties::on_lwChapters_itemSelectionChanged()
{
    QChapter *chapter = chapters[ui->lwChapters->selectedItems().at(0)];
    if (chapter != NULL)
    {
        ui->sbChapterNum->setValue(chapter->getNumber());
        ui->leChapterName->setText(chapter->getName());
    }
}

void VolumeProperties::on_pbOK_clicked()
{
    this->volume->setNumber(ui->sbVolumeNum->value());
    this->volume->setName(ui->leVolumeName->text());
    this->volume->setCover(*ui->lCover->pixmap());

    QList<QChapter*> chapters_list = chapters.values();
    foreach (QChapter *chapter, chapters_list)
        volume->updateChapter(chapter);

    emit UpdateVolumeInfo(volume);
    close();
}

void VolumeProperties::on_pbCancel_clicked()
{
    close();
}

void VolumeProperties::on_pbChangeCover_clicked()
{
    //
}

void VolumeProperties::on_pbAddChapter_clicked()
{
    QChapter *chapter = new QChapter();
    chapter->deserialize(QJsonDocument::fromJson("{\"id\":-1}").object());
    chapter->setVolumeId(volume->getId());
    chapter->setNumber(ui->sbChapterNum->value());
    chapter->setName(ui->leChapterName->text());

    QListWidgetItem *item = new QListWidgetItem();
    item->setText("[" + QString::number(chapter->getNumber()) + "] " + chapter->getName());
    ui->lwChapters->addItem(item);
    chapters[item] = chapter;
}

void VolumeProperties::on_pbChapterApply_clicked()
{
    QListWidgetItem *item = ui->lwChapters->selectedItems().at(0);
    QChapter *chapter = chapters[item];
    chapter->setNumber(ui->sbChapterNum->value());
    chapter->setName(ui->leChapterName->text());
    item->setText("[" + QString::number(chapter->getNumber()) + "] " + chapter->getName());
}
