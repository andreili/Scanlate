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

bool QScanlateServer::Login(QString username, QString password)
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
            return true;
        }
    }
    return false;
}

QJsonObject QScanlateServer::getUserInfo()
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=user_info");
    return query(url);
}

QJsonObject QScanlateServer::getProjectsList()
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=projects_list");
    return query(url);
}

QJsonObject QScanlateServer::query(QUrl url)
{
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
    }
    return obj;
}
