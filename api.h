#ifndef API_H
#define API_H

#include <QUrl>
#include <QJsonObject>

class API
{
public:
    API();

    QJsonObject query(QUrl url);
};

extern API api;

#endif // API_H
