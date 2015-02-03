#ifndef QSCANLATESERVER_H
#define QSCANLATESERVER_H

#include <QJsonObject>
#include <QObject>
#include <QUrl>

class QScanlateServer : public QObject
{
    Q_OBJECT
public:
    explicit QScanlateServer(QString server, QObject *parent = 0);

    bool Login(QString username, QString password);

    QJsonObject getUserInfo();
    QJsonObject getProjectsList();

private:
    QString server_url;
    QString token;

    QJsonObject query(QUrl url);

signals:

public slots:

};

#endif // QSCANLATESERVER_H
