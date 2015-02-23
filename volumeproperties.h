#ifndef VOLUMEPROPERTIES_H
#define VOLUMEPROPERTIES_H

#include <QDialog>
#include <QListWidgetItem>
#include "qvolume.h"

namespace Ui {
class VolumeProperties;
}

class VolumeProperties : public QDialog
{
    Q_OBJECT

public:
    explicit VolumeProperties(QVolume *volume, QWidget *parent = 0);
    ~VolumeProperties();

Q_SIGNALS:
    void UpdateVolumeInfo(QVolume *volume);

private slots:
    void on_lwChapters_itemSelectionChanged();

    void on_pbOK_clicked();

    void on_pbCancel_clicked();

    void on_pbChangeCover_clicked();

    void on_pbAddChapter_clicked();

    void on_pbChapterApply_clicked();

private:
    QVolume *volume;
    QMap<QListWidgetItem*,QChapter*> chapters;

    Ui::VolumeProperties *ui;
};

#endif // VOLUMEPROPERTIES_H
