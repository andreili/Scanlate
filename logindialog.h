#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "qscanlateserver.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

private:
    bool m_logged;

public:
    bool isLogged() { return m_logged; }

    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QScanlateServer* getServer() { return m_server; }

private:
    Ui::LoginDialog *ui;

    QScanlateServer *m_server;

public slots:
    void slotAcceptLogin();
};

#endif // LOGINDIALOG_H
