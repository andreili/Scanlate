#ifndef VOLUMEPROPERTIES_H
#define VOLUMEPROPERTIES_H

#include <QDialog>

namespace Ui {
class VolumeProperties;
}

class VolumeProperties : public QDialog
{
    Q_OBJECT

public:
    explicit VolumeProperties(QWidget *parent = 0);
    ~VolumeProperties();

private:
    Ui::VolumeProperties *ui;
};

#endif // VOLUMEPROPERTIES_H
