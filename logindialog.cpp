#include "logindialog.h"
#include "ui_logindialog.h"
#include "qscanlateserver.h"
#include <QPushButton>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->pb_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->pb_login, SIGNAL(clicked()), this, SLOT(slotAcceptLogin()));
    m_logged = false;
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::slotAcceptLogin()
{
    this->m_server = new QScanlateServer(ui->cb_server->currentText());

    switch (this->m_server->Login(ui->le_username->text(), ui->le_password->text()))
    {
    case QScanlateServer::LOGIN_OK:
        m_logged = true;
        close();
        break;
    case QScanlateServer::LOGIN_NO_CONNECTION:
        if ((QFile("./data/user.json").exists()) && (this->m_server->getMode() == QScanlateServer::OFFLINE))
        {
            switch (QMessageBox::question(this, QObject::tr("Ошибка"), QObject::tr("Отсутствует подключение к серверу!\nПродолжить в автономном режиме?")))
            {
            case QMessageBox::Yes:
                if ((QFile("./data/projects.json").exists()) && (QFile("./data/users.json").exists()))
                    m_logged = true;
                else
                {
                    QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Отсутствуют данные, необходимые для автономного режима!"));
                    m_logged = false;
                }
                break;
            default:
                m_logged = false;
                break;
            }
        }
        else
        {
            QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Отсутствует подключение к серверу!"));
            m_logged = false;
        }
        close();
        break;
    case QScanlateServer::LOGIN_FAIL:
        m_logged = false;
        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Неверное имя пользователя и/или пароль!"));
        break;
    }
}
