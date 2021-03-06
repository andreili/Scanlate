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

    void saveState();

private:
    Ui::MainWindow *ui;

    QString m_server;
    QString m_token;
    QString m_logged_as;

    QLabel *m_login_label;
    QLabel *m_network_label;
    QLabel *m_project_label;
    QLabel *m_chapter_label;

    QTreeWidgetItem *m_active_projects_root;
    QTreeWidgetItem *m_finished_projects_root;
    QTreeWidgetItem *m_inactive_projects_root;

    QScanlate *scanlate;
    QChapter *activeChapter;

    void setEnabledMenu(const QMenu &menu);

    void addNewProjectDialog();
    void projectPropertiesDialog(QScanlateProject *project);
    void setActiveProject(QScanlateProject *project);

    void addNewVolumeDialog();
    void volumePropertiesDialog(QVolume *volume);

    void setActiveChapter(QChapter *chapter);

Q_SIGNALS:
    void UpdateProgress(QString obj, int all, int completed);

public Q_SLOTS:
    void LoadProjectsList();
    void UpdateProjectInfo(QScanlateProject *project);
    void UpdateVolumeInfo(QVolume *volume);

    void LoadChapterFiles();

private slots:
    void on_twProjects_doubleClicked(const QModelIndex &index);
    void on_twProjects_customContextMenuRequested(const QPoint &pos);
    void on_twChapters_customContextMenuRequested(const QPoint &pos);
    void on_MainWindow_destroyed();
    void on_lwStyles_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_cbStyleFonts_currentFontChanged(const QFont &f);
    void on_sbStyleSize_valueChanged(int arg1);
    void on_cbItalic_stateChanged(int arg1);
    void on_cbBold_stateChanged(int arg1);
    void on_pbColor_clicked();
    void on_pbAdd_clicked();
    void on_pbSave_clicked();
};

#endif // MAINWINDOW_H
