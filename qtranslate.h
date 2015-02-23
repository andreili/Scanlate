#ifndef QTRANSLATE_H
#define QTRANSLATE_H

#include <QObject>
#include <QJsonObject>

class QTranslate : public QObject
{
    Q_OBJECT
public:
    explicit QTranslate(QObject *parent = 0);

    void deserialize(QJsonObject raw_data);
    QJsonObject serialize();

signals:

public slots:

};

#endif // QTRANSLATE_H
