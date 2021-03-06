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

QJsonObject QScanlateServer::UpdateProjectInfo(QJsonObject project_json)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=update_project");
    QUrlQuery params;
    params.addQueryItem("json", QJsonDocument(project_json).toJson());
    return query(url, params);
}

QJsonObject QScanlateServer::addNewProject(QJsonObject project_json)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=add_project");
    QUrlQuery params;
    params.addQueryItem("json", QJsonDocument(project_json).toJson());
    return query(url, params);
}

QJsonObject QScanlateServer::deleteProject(int project_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=del_project&project=" + QString::number(project_id));
    return query(url);
}

QJsonObject QScanlateServer::addNewVolume(QJsonObject volume_json, int project_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=add_volume&project=" + QString::number(project_id));
    QUrlQuery params;
    params.addQueryItem("json", QJsonDocument(volume_json).toJson());
    return query(url, params);
}

QJsonObject QScanlateServer::updateVolumeInfo(QJsonObject volume_json, int project_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=update_volume&project=" + QString::number(project_id));
    QUrlQuery params;
    params.addQueryItem("json", QJsonDocument(volume_json).toJson());
    return query(url, params);
}

QJsonObject QScanlateServer::deleteVolume(int volume_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=del_volume&volume=" + QString::number(volume_id));
    return query(url);
}

QJsonObject QScanlateServer::deleteChapter(int chapter_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=del_chapter&chapter=" + QString::number(chapter_id));
    return query(url);
}

QJsonObject QScanlateServer::getStylesForProject(int project_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=get_styles&project=" + QString::number(project_id));
    return query(url);
}

QJsonObject QScanlateServer::getTranslateForChapter(int chapter_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=get_translate&chapter=" + QString::number(chapter_id));
    return query(url);
}

QJsonObject QScanlateServer::getRAWForChapter(int chapter_id)
{
    QUrl url(server_url + "/data.php");
    url.setQuery("query=get_raw_list&chapter=" + QString::number(chapter_id));
    return query(url);
}

bool QScanlateServer::loadFile(QString URL, QString fileName)
{
    // TODO
    return true;
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

QJsonObject QScanlateServer::query(QUrl url, QUrlQuery params)
{
    if (token.length())
        url.setQuery(url.query() + "&token=" + token);

    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req(url);
    QNetworkReply *reply = mgr.post(req, params.toString().toUtf8());
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
