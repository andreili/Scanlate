#include "qscanlateuser.h"
#include <QMessageBox>

QScanlateUser::QScanlateUser(QObject *parent) :
    QObject(parent)
{
}

#define SQL_DATE_FORMAT "yyyy-MM-dd hh:mm:ss"

QJsonObject QScanlateUser::serialize()
{
    QJsonObject ret_val;
    ret_val["id"] = this->id;
    ret_val["login"] = this->login;
    ret_val["reg_date"] = this->regDate.toString(SQL_DATE_FORMAT);
    QJsonObject roles_obj;
    int i = 0;
    foreach (Role role, this->roles)
    {
        roles_obj[QString::number(++i)] = (int)role;
    }
    ret_val["roles"] = roles_obj;

    return ret_val;
}

void QScanlateUser::deserialize(QJsonObject raw_data)
{
    this->id = raw_data["id"].toInt();
    this->login = raw_data["login"].toString();
    this->regDate = QDateTime::fromString(raw_data["reg_date"].toString(),SQL_DATE_FORMAT);
    QJsonObject roles = raw_data["roles"].toObject();
    foreach (QJsonValue role_json, roles) {
        Role role = (Role)role_json.toObject()["role_id"].toInt();
        this->roles.append(role);
    }
}

bool QScanlateUser::isModerator()
{
    foreach (Role role, this->roles)
        if ((role == Role::Admin) || (role == Role::TeamLider))
            return true;
    return false;
}
