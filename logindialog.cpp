#include "logindialog.h"
#include "ui_logindialog.h"
#include "qscanlateserver.h"
#include <QPushButton>
#include <QUrl>
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

    if (this->m_server->Login(ui->le_username->text(), ui->le_password->text()))
    {
        m_logged = true;
        close();
    }
    else
    {
        m_logged = false;
        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Неверное имя пользователя и/или пароль!"));
    }
}
