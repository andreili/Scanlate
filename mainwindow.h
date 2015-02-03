#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QLabel>
#include <QTreeWidgetItem>
#include "qscanlate.h"
#include "qscanlateserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QScanlateServer *server, QWidget *parent = 0);
    ~MainWindow();

    void setUser(QString login, QString token);

private:
    Ui::MainWindow *ui;

    QString m_server;
    QString m_token;
    QString m_logged_as;

    QLabel *m_login_label;

    QTreeWidgetItem *m_active_projects_root;
    QTreeWidgetItem *m_finished_projects_root;
    QTreeWidgetItem *m_inactive_projects_root;

    QScanlate *scanlate;

Q_SIGNALS:
    void UpdateProgress(QString obj, int all, int completed);

public Q_SLOTS:
    void LoadProjectsList();
};

#endif // MAINWINDOW_H
