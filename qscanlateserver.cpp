#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QApplication>
#include <QMessageBox>
#include "qscanlateserver.h"

QScanlateServer::QScanlateServer(QString server, QObject *parent) :
    QObject(parent)
{
    server_url = server;
}

QScanlateServer::LoginResult QScanlateServer::Login(QString username, QString password)
{
    QUrl url(server_url + "/login.php");
    url.setQuery(QString("login=%1&password=%2").arg(username,
                                                     QString(QCryptographicHash::hash(password.toLatin1(),
                                                                                      QCryptographicHash::Md5).toHex())));
    QJsonObject json_reply = query(url);

    if (!json_reply.empty())
    {
        if ((json_reply["error"].toInt() == 0) && (json_reply.contains("token")))
        {
            token = json_reply["token"].toString();
            mode = NORNAL;
            return LOGIN_OK;
        }
        else if (json_reply["error"].toInt() == 1024)
        {
            mode = OFFLINE;
            return LOGIN_NO_CONNECTION;
        }
    }
    return LOGIN_FAIL;
}

void QScanlateServer::setToken(QString new_token)
{
    this->token = new_token;
}

QJsonObject QScanlateServer::getUserInfo()
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=user_info");
    return query(url);
}

QJsonObject QScanlateServer::getUsersList()
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=users_list");
    return query(url);
}

QJsonObject QScanlateServer::getProjectsList()
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=projects_list");
    return query(url);
}

QJsonObject QScanlateServer::getChaptersList(int project_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=chapters_list&project=" + QString::number(project_id));
    return query(url);
}

QJsonObject QScanlateServer::query(QUrl url)
{
    if (token.length())
        url.setQuery(url.query() + "&token=" + token);

    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req(url);
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();
    //QObject::disconnect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QJsonObject obj;
    if (reply->error() == QNetworkReply::NoError)
    {
        obj = QJsonDocument::fromJson(reply->readAll()).object();
        if (!obj.empty())
        {
            if ((obj["error"].toInt() == 0))
            {
                // TODO: Handling errors
            }
        }
    }
    else
    {
        QMessageBox::critical(NULL, QObject::tr("Ошибка подключения"), reply->errorString());
        obj["error"] = 1024;
    }
    return obj;
}
