#include "qscanlateuser.h"
#include <QMessageBox>

QScanlateUser::QScanlateUser(QObject *parent) :
    QObject(parent)
{
}

  QScanlateUser::QScanlateUser(QJsonObject raw_data, QObject *parent) :
      QObject(parent)
{
    this->login = raw_data["login"].toString();
    this->regDate = QDateTime::fromString(raw_data["reg_date"].toString());
    QJsonObject roles = raw_data["roles"].toObject();
    foreach (QJsonValue role_json, roles) {
        Role role = (Role)role_json.toObject()["role_id"].toInt();
        this->roles.append(role);
    }
}

void QScanlateUser::setInfo(QJsonObject raw_data)
{
    //
}

bool QScanlateUser::isModerator()
{
    foreach (Role role, this->roles)
        if ((role == Role::Admin) || (role == Role::TeamLider))
            return true;
    return false;
}
