#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QDialog>

namespace Ui {
class LoadingWindow;
}

class LoadingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadingWindow(QWidget *parent = 0);
    ~LoadingWindow();

    void Start();

private:
    Ui::LoadingWindow *ui;

Q_SIGNALS:
    void loadingProc();

public Q_SLOTS:
    void UpdateProgress(QString obj, int all, int completed);
};

#endif // LOADINGWINDOW_H
