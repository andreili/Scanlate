#include "volumeproperties.h"
#include "ui_volumeproperties.h"

VolumeProperties::VolumeProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VolumeProperties)
{
    ui->setupUi(this);
}

VolumeProperties::~VolumeProperties()
{
    delete ui;
}
