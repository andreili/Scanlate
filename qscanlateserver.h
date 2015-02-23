#ifndef QSCANLATESERVER_H
#define QSCANLATESERVER_H

#include <QJsonObject>
#include <QUrlQuery>
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

    QJsonObject UpdateProjectInfo(QJsonObject project_json);
    QJsonObject addNewProject(QJsonObject project_json);
    QJsonObject deleteProject(int project_id);

    QJsonObject addNewVolume(QJsonObject volume_json, int project_id);
    QJsonObject updateVolumeInfo(QJsonObject volume_json, int project_id);
    QJsonObject deleteVolume(int volume_id);
    QJsonObject deleteChapter(int chapter_id);

    QJsonObject getStylesForProject(int project_id);
    QJsonObject getTranslateForChapter(int chapter_id);
    QJsonObject getRAWForChapter(int chapter_id);

    bool loadFile(QString URL, QString fileName);

private:
    NetworkMode mode;
    QString server_url;
    QString token;

    QJsonObject query(QUrl url);
    QJsonObject query(QUrl url, QUrlQuery params);

signals:

public slots:

};

#endif // QSCANLATESERVER_H
