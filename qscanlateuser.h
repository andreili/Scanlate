#ifndef QSCANLATEUSER_H
#define QSCANLATEUSER_H

#include <QObject>
#include <QJsonObject>
#include <QDateTime>

class QScanlateUser : public QObject
{
    Q_OBJECT
public:
    explicit QScanlateUser(QObject *parent = 0);
    explicit QScanlateUser(QJsonObject raw_data, QObject *parent = 0);

    enum Role
    {
        Admin = 0,
        User = 1,
        TeamLider = 2,
        Scanner = 3,
        Translator = 4,
        ProofReader = 5,
        Cleaner = 6,
        Typer = 7,
        Beta = 8
    };

    void setInfo(QJsonObject raw_data);

    bool isModerator();
    QString getLogin() { return this->login; }

private:
    QString login;
    QDateTime regDate;
    QList<Role> roles;

signals:

public slots:

};

#endif // QSCANLATEUSER_H
