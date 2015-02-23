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
    void on_pbChangeCover_clicked();

    void on_pbCancle_clicked();

    void on_pbOK_clicked();

Q_SIGNALS:
    void UpdateProjectInfo(QScanlateProject *project);

private:
    Ui::ProjectProperties *ui;

    QScanlateProject *project;
    bool editable;
};

#endif // PROJECTPROPERTIES_H
