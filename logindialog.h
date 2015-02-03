#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

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

    QString getToken() { return m_token; }
    QString getUser() { return m_login; }
    QString getServer() { return m_server; }

private:
    Ui::LoginDialog *ui;

    QString m_server;
    QString m_token;
    QString m_login;

public slots:
    void slotAcceptLogin();
};

#endif // LOGINDIALOG_H
