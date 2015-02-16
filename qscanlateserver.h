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

    enum LoginResult
    {
        LOGIN_OK,
        LOGIN_FAIL,
        LOGIN_NO_CONNECTION
    };

    enum NetworkMode
    {
        NORNAL,
        OFFLINE
    };

    LoginResult Login(QString username, QString password);
    NetworkMode getMode() { return this->mode; }

    QString getToken() { return this->token; }
    void setToken(QString new_token);

    QJsonObject getUserInfo();
    QJsonObject getUsersList();
    QJsonObject getProjectsList();
    QJsonObject getChaptersList(int project_id);

private:
    NetworkMode mode;
    QString server_url;
    QString token;

    QJsonObject query(QUrl url);

signals:

public slots:

};

#endif // QSCANLATESERVER_H
