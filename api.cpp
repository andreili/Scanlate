#include "api.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QApplication>
#include <QMessageBox>

API api;

API::API()
{
}

QJsonObject API::query(QUrl url)
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
