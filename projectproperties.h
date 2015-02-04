#ifndef PROJECTPROPERTIES_H
#define PROJECTPROPERTIES_H

#include <QDialog>
#include "qscanlateproject.h"

namespace Ui {
class ProjectProperties;
}

class ProjectProperties : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectProperties(QScanlateProject *project, bool editable, QWidget *parent = 0);
    ~ProjectProperties();

private slots:
    void on_pbChangeCover_pressed();

    void on_pbChangeCover_clicked();

private:
    Ui::ProjectProperties *ui;
};

#endif // PROJECTPROPERTIES_H
