#include "volumeproperties.h"
#include "ui_volumeproperties.h"

VolumeProperties::VolumeProperties(QVolume *volume, QChapter *chapter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VolumeProperties)
{
    ui->setupUi(this);

    this->volume = volume;
    this->chapter = chapter;
}

VolumeProperties::~VolumeProperties()
{
    delete ui;
}
