#ifndef VOLUMEPROPERTIES_H
#define VOLUMEPROPERTIES_H

#include <QDialog>
#include "qvolume.h"

namespace Ui {
class VolumeProperties;
}

class VolumeProperties : public QDialog
{
    Q_OBJECT

public:
    explicit VolumeProperties(QVolume *volume, QChapter *chapter, QWidget *parent = 0);
    ~VolumeProperties();

Q_SIGNALS:
    void UpdateVolumeInfo(QVolume *volume);

private:
    QVolume *volume;
    QChapter *chapter;

    Ui::VolumeProperties *ui;
};

#endif // VOLUMEPROPERTIES_H
