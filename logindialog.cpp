#include "logindialog.h"
#include "ui_logindialog.h"
#include "api.h"
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
    QString server = ui->cb_server->currentText();
    QString username = ui->le_username->text();
    QString password = ui->le_password->text();

    QUrl url(server + "/login.php");
    url.setQuery(QString("login=%1&password=%2").arg(username,
                                                     QString(QCryptographicHash::hash(password.toLatin1(),
                                                                                      QCryptographicHash::Md5).toHex())));
    QJsonObject json_reply = api.query(url);

    if (!json_reply.empty())
    {
        if ((json_reply["error"].toInt() == 0) && (json_reply.contains("token")))
        {
            m_server = server;
            m_logged = true;
            m_token = json_reply["token"].toString();
            m_login = ui->le_username->text();
            close();
        }
    }
    else
    {
        m_logged = false;
        QMessageBox::critical(this, QObject::tr("Ошибка"), QObject::tr("Неверное имя пользователя и/или пароль!"));
    }
}
